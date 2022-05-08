unit MIDFiles;

interface
const
  f_counter = %10000000; // prevents counting
  f_tick    = %01000000; // tic indicator
  f_flags   = %11000000; // flags mask
  f_ratio   = %00001111; // timer divider mask

  _trkRegs  = $e0;       // buffer for trach processing

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
  fps:Byte;
  fsd:Byte;
  tickDiv:Word;
  ms_per_qnote:longint;
  tactNum:Byte;
  tactDenum:Byte;
  ticks_per_qnote:Byte;
  ticks_per_32nd:Byte;
  BPM:Word;

  oldTimerVec:Pointer;

  _totalTicks:TDeltaTime  absolute $f0; {f0, f1, f2, f3}
  _subCnt:Byte            absolute $f4;
  _timerStatus:Byte       absolute $f5;
  _tmp:Byte               absolute $f6;

// the order of the registers MUST be the same as in the TMIDTrack record!!!
  _ptr:^Byte              absolute _trkRegs;   {16-bits}
  _adr:Word               absolute _trkRegs;   // must be the same address as _ptr!!
  _trackTime:TDeltaTime   absolute _trkRegs+2; {32-bits}
  _skipDelta:Boolean      absolute _trkRegs+6;
  _event:Byte             absolute _trkRegs+7;

function LoadMID(fn:PString):Boolean;
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

function wordBI(var bi:TByteArray):Word;
var
  ResultPTR:^Byte;

begin
  ResultPTR:=@Result;
  ResultPTR^:=BI[1]; inc(ResultPTR);
  ResultPTR^:=BI[0];
end;

function longBI(var bi:TByteArray):longint;
var
  ResultPTR:^Byte;
  i:Byte;

begin
  ResultPTR:=@Result;
  for i:=3 downto 0 do
  begin
    ResultPTR^:=BI[i];
    inc(ResultPTR);
  end;
end;

{
  memory under XDOS 2.43N:

  $0700..$1df0 XDOS
  $4000..$7fff Free mem - extended memory bank
  $8000..$9c1f Free mem
  $9c20..$9c3f Display List
  $9c40..$9fff Screen
  $a000..$bfff Free mem
  $c000..$d7ff Free mem (under ROM)
  $d800..$dfff Free mem (under ROM)
  $e000..$e3ff Font set
  $e400..$fffd Free mem (under ROM)

  free memory gaps in page align:

  $4000..$9bff
  $a000..$bfff
  $c000..$d7ff
  $e400..$ff00

  total free: AF00 (44800)
}

{$I memboundcheck.inc}

//
//
//

function LoadMID(fn:PString):boolean;
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
    WriteLn(#155,'I/O Error #',IOResult);
    Close(f);
    exit(false);
  end;
  trackCount:=0; nTracks:=255;
  while (IOResult<128) and (not EOF(F)) and (trackCount<nTracks) do
  begin
    BlockRead(f,chunkTag,4,v);
    if (v<>4) then break;
    Len:=ReadLongBI;
    if chunkTag=TAG_MTHD then
    begin
      format:=readWordBI;
      nTracks:=readWordBI;
{$IFDEF DEBUG}
      WriteLn('Format: ',format);
      WriteLn('Tracks: ',nTracks);
{$ENDIF}
      v:=readWordBI;
      if (v and $8000)<>0 then
      begin
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

      nTrkRec^.ptr:=MIDData;
      nTrkRec^.trackTime:=0;
      nTrkRec^.skipDelta:=false;

      while Len>0 do
      begin
        _adr:=Word(MIDData);
        memBoundCheck;
        MIDData:=Pointer(_adr);
        if len>128 then loadSize:=128 else loadSize:=len;
        endAdr:=_adr+loadSize;
        if (endAdr>=$9c00) and (endAdr<$A000) then
          loadSize:=$9c00-_adr
        else if (endAdr>=$d000) and (endAdr<$d800) then
          loadSize:=$d000-_adr
        else if (endAdr>=$e000) and (endAdr<$e400) then
          loadSize:=$e000-_adr
        else if (endAdr>=$ff00) then
          loadSize:=$ff00-_adr;
        if loadSize=0 then continue;

        BlockRead(f,RBuf,loadSize,v);
        if v<>loadSize then exit(false);

        move(@RBuf,MIDData,loadSize);
        inc(MIDData,v);
        Dec(Len,v);
      end;

      inc(nTrkRec,1);
    end;
    Write(#156);
  end;
  Close(f);
  result:=true;
end;

function ProcessTrack:TDeltaTime;
var
  flagSysEx:Boolean;
  deltaTime:TDeltaTime;
  msgLen:Byte;
  nTempo:Longint;

  procedure readB; Inline;
  begin
    _tmp:=_ptr^;
    inc(_adr);
    memBoundCheck;
  end;

{$IFDEF USE_FIFO}
  procedure readB2FB; Inline;
  begin
    FIFO_Byte:=_ptr^;
    inc(_adr);
    memBoundCheck;
  end;
{$ELSE}
  procedure readB2FB; Inline;
  begin
    MC_Byte:=_ptr^;
    inc(_adr);
    memBoundCheck;
  end;
{$ENDIF}

  procedure getData(p:PByte; size:Byte);
  begin
    while size>0 do
    begin
      p^:=_ptr^;
      inc(_adr);
      memBoundCheck;
      inc(p);
      dec(size);
    end;
  end;

{$I readVarL.inc}

  // function readVarL:TDeltaTime;
  // begin
  //   result:=0;
  //   repeat
  //     ReadB;
  //     result:=result shl 7;
  //     result:=result or (_tmp and $7f);
  //   until (_tmp and $80=0);
  // end;


  function readT:longint;
  var
    ResultPTR:^Byte;

  begin
    ResultPTR:=@Result;
    getData(BI,3);
    ResultPTR^:=BI[2]; inc(ResultPTR);
    ResultPTR^:=BI[1]; inc(ResultPTR);
    ResultPTR^:=BI[0];
  end;

begin
  deltaTime:=0;
  repeat
    if not _skipDelta then
    begin
      deltaTime:=readVarL;
      if deltaTime>0 then break;
    end
    else
      _skipDelta:=false;

    if _ptr^ and $80<>0 then
    begin
      ReadB; _event:=_tmp;
    end;

    case _event of
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
          _tmp:=readVarL;
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
      $FF: // Meta events
        begin
          readB2FB;
          msgLen:=readVarL;
{$IFDEF USE_FIFO}
          case FIFO_Byte of
{$ELSE}
          case MC_Byte of
{$ENDIF}
            $2f: // end of track
              deltaTime:=-1;
            $51: // tempo
              begin
                nTempo:=readT;
                if nTempo<>ms_per_qnote then
                begin
                  ms_per_qnote:=nTempo;
                  setTempo;
                end;
              end;
            $58: // time signature
              begin
                readB; tactNum:=_tmp;
                readB; tactDenum:=_tmp;
                readB; ticks_per_qnote:=_tmp;
                readB; ticks_per_32nd:=_tmp;
                setTempo;
              end;
          else
            _tmp:=msgLen;
            while _tmp>0 do
            begin
              dec(_tmp);
              inc(_adr);
              memBoundCheck;
            end;
          end;
        end;
    end;
  until deltaTime=-1;
  _skipDelta:=true;
  result:=deltaTime;
end;


procedure int_play; Interrupt; Assembler;
asm
    lda _timerStatus
    bmi skip

    and #f_ratio
    cmp _subCnt
    bne incSubCounter

    lda _timerStatus
    ora #f_tick
    sta _timerStatus

    lda #1
    sta _subCnt

    inc _totalTicks
    bne skip
    inc _totalTicks+1
    bne skip
    inc _totalTicks+2
    bne skip
    inc _totalTicks+3
    bne skip

incSubCounter:
    inc _subCnt

skip:

  pla
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
  _ratio:=1+trunc(ratio); //ratio-frac(ratio));
  if _ratio>15 then ratio:=15;

  // set timer ratio
  _timerStatus:=(_timerStatus and f_flags) or _ratio;

  // calc frequency divider for base timer
  fdiv:=round(64000/(freq*_ratio));

  // calc tempo (Beats Per Minutes)
  BPM:=60000000 div ms_per_qnote;

// {$IFDEF DEBUG}
//   writeLn();
// {$ENDIF}

  setIntVec(iTim1,oldTimerVec);
  setIntVec(iTim1,@int_play,0,fdiv);
end;
{
asm
  sei
  lda FDIV
  sta $d200
  lda $10
  ora #$01
  sta $10
  sta $d20e
  sta $d209
  cli
end;
}
initialization
  oldTimerVec:=nil;
  tickDiv:=384;
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
  ms_per_qnote:=500000;
  _timerStatus:=0;
  inc(ms_per_qnote);
  getIntVec(iTim1,oldTimerVec);
end.