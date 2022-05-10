unit MIDFiles;

interface
const
  f_counter = %10000000; // prevents counting
  f_tick    = %01000000; // tic indicator
  f_flags   = %11000000; // flags mask
  f_ratio   = %00001111; // timer divider mask

  _trkRegs  = $e0;       // ZP registers for track processing

// MID file code formats
// __this player only supports 0 and 1 formats__
  MID_0 = 0;
  MID_1 = 1;

  ERR_UNSUPPORTED_FORMAT = 100;
  ERR_NOT_ENOUGHT_MEMORY = 101;

type
  TDeltaTime = Longint;
  TByteArray = Array[0..0] of Byte;
  TBigIndian = Array[0..3] of Byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    ptr:Pointer;
    trackTime:TDeltaTime;
    skipDelta:boolean;
    _event:Byte;
  end;

var
  MIDData:Pointer;
  MIDTracks:TByteArray;
  format:Word;
  nTracks:Word;
  tickDiv:Word;
  ms_per_qnote:longint;
// The following variables are informational only
// they are not used in the file playback process
{$IFDEF USE_SUPPORT_VARS}
  fps:Byte;
  fsd:Byte;
  tactNum:Byte;
  tactDenum:Byte;
  ticks_per_qnote:Byte;
  ticks_per_32nd:Byte;
  BPM:Word;
{$ENDIF}
  oldTimerVec:Pointer;

// Zero page work registers
  _totalTicks:TDeltaTime  absolute $f0; {f0, f1, f2, f3}
  _subCnt:Byte            absolute $f4;
  _timerStatus:Byte       absolute $f5;
  _delta:Longint          absolute $f6; {f7, f8, f9, fa}
  _tmp:Byte               absolute $f6; // must by the same address as _delta!!

// the order of the registers MUST be the same as in the TMIDTrack record!!!
  _ptr:^Byte              absolute _trkRegs;   {16-bits}
  _adr:Word               absolute _trkRegs;   // must be the same address as _ptr!!
  _trackTime:TDeltaTime   absolute _trkRegs+2; {32-bits}
  _skipDelta:Boolean      absolute _trkRegs+6;
  _event:Byte             absolute _trkRegs+7;

//
function LoadMID(fn:PString):shortint;
function ProcessTrack:TDeltaTime;
procedure setTempo;

implementation
Uses
  {$IFDEF USE_FIFO}MIDI_FIFO,{$ENDIF}
  MC6850,
  sysutils
  ;

type
  TTag = Longint;

const
  TAG_MTHD : TTag = $6468544D;
  TAG_MTRK : TTag = $6B72544D;

var
  BI:TBigIndian;
  RBuf:TByteArray absolute $600;

{$i int_timer.inc}
{$i memboundcheck.inc}
{$i bigindian.inc}

//
//
//

function LoadMID(fn:PString):shortint;
var
  f:File;
  trackCount:Word;
  chunkTag:TTag;
  v,top,endAdr:Word;
  Len:Longint;
  loadSize:Byte;
  nTrkRec:PMIDTrack;

  function ReadWordBI:Word;
  begin
    blockRead(f,BI,2);
    result:=wordBI(BI);
  end;

  function ReadLongBI:longint;
  begin
    blockRead(f,BI,4);
    result:=longBI(bi);
  end;

begin
  nTrkRec:=@MIDTracks;
  WriteLn('Open file ',fn);
{$I-}
  Assign(f,fn);
  Reset(f,1);
  if IOResult>127 then
  begin
    result:=IOResult;
    Close(f);
    exit;
  end;

  trackCount:=0;
  nTracks:=255;
  _adr:=Word(MIDData);

  while (IOResult=1) and (not EOF(F)) and (trackCount<nTracks) do
  begin
    BlockRead(f,chunkTag,4,v);
    if (v<>4) then break;
    Len:=ReadLongBI;
    if chunkTag=TAG_MTHD then
    begin
      format:=readWordBI;
      if (format<>MID_0) and (format<>MID_1) then exit(ERR_UNSUPPORTED_FORMAT);
      nTracks:=readWordBI;
{$IFDEF DEBUG}
      WriteLn('Format: ',format);
      WriteLn('Tracks: ',nTracks);
{$ENDIF}
      v:=readWordBI;
      if (v and $8000)<>0 then
      begin
{$IFDEF USE_SUPPORT_VARS}
        fps:=(v shr 8) and $7F;
        case fps of
          $E8 : fps:=24;
          $E7 : fps:=25;
          $E3 : fps:=29;
          $E2 : fps:=30;
        end;
        fsd:=v and $ff;
  {$IFDEF DEBUG}
        WriteLn('FPS: ',FPS);
        WriteLn('Frame divisor: ',fsd);
  {$ENDIF}
{$ENDIF}
      end
      else
      begin
        tickDiv:=v and $7fff;
{$IFDEF DEBUG}
        WriteLn('TickDiv: ',tickDiv);
{$ENDIF}
      end;
    end
    else if chunkTag=TAG_MTRK then
    begin
      inc(trackCount);
      Write('Track: ',trackCount,'/',nTracks,'...');

      nTrkRec^.ptr:=pointer(_adr);
      nTrkRec^.trackTime:=0;
      nTrkRec^.skipDelta:=false;

      while Len>0 do
      begin
        // _adr:=Word(MIDData);
        memBoundCheck; if IOResult<>1 then exit(ERR_NOT_ENOUGHT_MEMORY);
        // MIDData:=Pointer(_adr);
        if len>128 then loadSize:=128 else loadSize:=len;
        endAdr:=_adr+loadSize;

        if (endAdr>=$9c00) and (endAdr<$a000) then
          loadSize:=$9c00-_adr
        else if (endAdr>=$d000) and (endAdr<$d800) then
          loadSize:=$d000-_adr
        else if (endAdr>=$e000) and (endAdr<$e400) then
          loadSize:=$e000-_adr
        else if (endAdr>=$ff00) then
          loadSize:=$ff00-_adr;
        if loadSize=0 then continue;

        BlockRead(f,RBuf,loadSize,v);
        if v<>loadSize then exit(IOResult);

        move(@RBuf,_ptr,loadSize);
        inc(_adr,v);
        Dec(Len,v);
      end;

      inc(nTrkRec,1);
    end;
    Write(#156);
  end;
  Close(f);
  result:=0;
end;

function ProcessTrack:TDeltaTime;
var
  flagSysEx:Boolean;
  msgLen:Byte;

  procedure readB; Inline;
  begin
    _tmp:=_ptr^;
    inc(_adr);
    memBoundCheck;
  end;

  procedure readB2FB; Inline;
  begin
{$IFDEF USE_FIFO}
    FIFO_Byte:=_ptr^;
{$ELSE}
    MC_Byte:=_ptr^;
{$ENDIF}
    inc(_adr);
    memBoundCheck;
  end;

{$i readVarL.inc}
{$i read24bits.inc}

begin
  _delta:=0;
  repeat
    if not _skipDelta then
    begin
      readVarL;
      if _delta>0 then break;
    end
    else
      _skipDelta:=false;

    if _ptr^ and $80<>0 then
    begin
      ReadB; _event:=_tmp;
    end;

    case _event of
      // events from $80 to $F7 are sent to MIDI port
      $80..$BF,
      $E0..$EF: // two parameters for event
        begin
{$IFDEF USE_FIFO}
          FIFO_Byte:=_event; FIFO_WriteByte;
          readB2FB; FIFO_WriteByte;
          readB2FB; FIFO_WriteByte;
{$ELSE}
          MC_Byte:=_event; MC6850_Send2;
          readB2FB; MC6850_Send2;
          readB2FB; MC6850_Send2;
{$ENDIF}
        end;
      $C0..$DF: // one parameter for event
        begin
{$IFDEF USE_FIFO}
          FIFO_Byte:=_event; FIFO_WriteByte;
          readB2FB; FIFO_WriteByte;
{$ELSE}
          MC_Byte:=_event; MC6850_Send2;
          readB2FB; MC6850_Send2;
{$ENDIF}
        end;
      $F0..$F7: // SysEx Event
        begin
          readVarL;
{$IFDEF USE_FIFO}
          FIFO_Byte:=_event; FIFO_WriteByte;
{$ELSE}
          MC_Byte:=_event; MC6850_Send2;
{$ENDIF}
          while _tmp>0 do
          begin
            readB2FB;
{$IFDEF USE_FIFO}
            FIFO_WriteByte;
{$ELSE}
            MC6850_Send2;
{$ENDIF}
            dec(_tmp);
          end;
{$IFDEF USE_FIFO}
          if FIFO_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
{$ELSE}
          if MC_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
{$ENDIF}
        end;

      // Meta events are not directly part of the MIDI protocol
      $FF:
        begin
          readB2FB; // fetch Meta event numer
          readVarL; // fetch data size
{$IFDEF USE_FIFO}
          case FIFO_Byte of
{$ELSE}
          case MC_Byte of
{$ENDIF}
            $2f: // end of track
              _delta:=-1;
            $51: // set tempo
              begin
                read24;
                if _delta<>ms_per_qnote then
                begin
                  ms_per_qnote:=_delta;
                  setTempo;
                end;
              end;
{$IFDEF USE_SUPPORT_VARS}
            $58: // time signature
              begin
                readB; tactNum:=_tmp;
                readB; tactDenum:=_tmp;
                readB; ticks_per_qnote:=_tmp;
                readB; ticks_per_32nd:=_tmp;
              end;
{$ENDIF}
          else
          // any orther meta event are skipped
          begin
            // writeLn(_tmp);
            while _tmp>0 do
            begin
              dec(_tmp);
              inc(_adr);
              memBoundCheck;
            end;
          end;
          end;
        end;
    end;
  until _delta=-1;
  _skipDelta:=true;
  result:=_delta;
end;

procedure setTempo;
var
  freq:single;
  _freq:longint;
  fdiv:Byte;
  ratio:Single;
  _ratio:Byte;

begin
  _freq:=ms_per_qnote div tickDiv;
  freq:=_freq/1000000;
  freq:=1/freq;
  // freq:=1/((tickDiv*ticks_per_qnote)/1000000)*8;

  ratio:=250.9803/freq;
  _ratio:=1+trunc(ratio);
  if _ratio>15 then ratio:=15;

  // store timer ratio in status
  _timerStatus:=(_timerStatus and f_flags) or _ratio;

  // calc frequency divider for base timer
  fdiv:=round(64000/(freq*_ratio));

{$IFDEF USE_SUPPORT_VARS}
  // calc tempo (Beats Per Minutes)
  BPM:=60000000 div ms_per_qnote;
{$ENDIF}
  setIntVec(iTim1,@int_timer,0,fdiv);
end;

initialization
  oldTimerVec:=nil;
  tickDiv:=384;
  ms_per_qnote:=500000;
{$IFDEF USE_SUPPORT_VARS}
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
{$ENDIF}
  _timerStatus:=0;
  getIntVec(iTim1,oldTimerVec);
end.