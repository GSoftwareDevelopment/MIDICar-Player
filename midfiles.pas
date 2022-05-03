unit MIDFiles;

interface
const
  freq_ratio=2;
  f_counter = %10000000;
  f_tick    = %01000000;

type
  TDeltaTime = word;
  TByteArray = Array[0..0] of byte;
  TBigIndian = Array[0..3] of byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    ptr:Pointer;
    deltaTime:TDeltaTime;
    skipDelta,
    EOT:Boolean;
    _event:byte;
  end;

var
  MIDData:Pointer;
  MIDTracks:TByteArray;
  format:word;
  nTracks:word;
  fps:byte;
  fsd:byte;
  tickDiv:word;
  ms_per_qnote:longint;
  tactNum:byte;
  tactDenum:byte;
  ticks_per_qnote:byte;
  ticks_per_32nd:byte;
  BPM:Word;

  // VIMIRQ:Pointer absolute $216;
  // oldIRQ:Pointer;
  oldTimerVec:Pointer;

  _timerStatus:byte absolute $df;
{ 76543210
  ||
  |+-- tick flag (f_tick)
  +--- counter flag (f_counter)
}

  _subCnt:byte absolute $f2;
  totalTicks:longint absolute $f3;

function LoadMID(fn:PString):Boolean;
function GetTrackData(track:PMIDTrack):TDeltaTime;
procedure setTempo(nTempo:longint);

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
  TrackData:^Byte absolute $f7;

function wordBI(var bi:TByteArray):word;
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
  i:byte;

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

procedure memBoundCheck(var adr:word);
begin
  if adr=$9c00 then
    adr:=$A000
  else if adr=$d000 then
    adr:=$d800
  else if adr=$E000 then
    adr:=$e400
  else if adr=$FF00 then
  begin
    WriteLn('Not enought memory.');
    halt(2);
  end;
end;

//
//
//

function LoadMID(fn:PString):boolean;
var
  f:File;
  trackCount:word;
  chunkTag:TTag;
  v,top,loadAdr,endAdr:word;
  Len:Longint;
  loadSize:byte;
  nTrkRec:PMIDTrack;

  function ReadWordBI:word;
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
      nTrkRec^.deltaTime:=0;
      nTrkRec^.skipDelta:=false;
      nTrkRec^.EOT:=false;

      while Len>0 do
      begin
        loadAdr:=word(MIDData);
        memBoundCheck(loadAdr);
        MIDData:=pointer(loadAdr);
        if len>128 then loadSize:=128 else loadSize:=len;
        endAdr:=loadAdr+loadSize;
        if (endAdr>=$9c00) and (endAdr<$A000) then
          top:=$9c00
        else if (endAdr>=$d000) and (endAdr<$d800) then
          top:=$d000
        else if (endAdr>=$e000) and (endAdr<$e400) then
          top:=$e000
        else top:=0;
        if top<>0 then
          loadSize:=top-loadAdr;
        if loadSize=0 then continue;
        // WriteLn(intToHex(loadAdr,4),' ',intToHex(loadSize,4));

        BlockRead(f,RBuf,loadSize,v);
        if v<>loadSize then halt(1);
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

function GetTrackData(track:PMIDTrack):TDeltaTime;
var
  flagSysEx:Boolean;
  DeltaTime,msgLen:TDeltaTime;
  v,Event,Meta:Byte;
  // readBuf:Boolean;
  adr:word;

  function ReadB:Byte;
  begin
    adr:=word(TrackData);
    memBoundCheck(adr);
    TrackData:=pointer(adr);
    result:=TrackData^;
    inc(TrackData);
  end;

  procedure getData(var buf:TByteArray; size:byte);
  var
    i:byte;

  begin
    i:=0;
    while size>0 do
    begin
      adr:=word(TrackData);
      memBoundCheck(adr);
      TrackData:=pointer(adr);
      buf[i]:=TrackData^;
      inc(TrackData);
      inc(i);
      dec(size);
    end;
  end;

  procedure skip(n:byte);
  begin
    while n>0 do
    begin
      adr:=word(TrackData);
      memBoundCheck(adr);
      TrackData:=pointer(adr);
      dec(n);
      inc(TrackData);
    end;
  end;

  function getVarLong:TDeltaTime;
  // var
  //   v:byte;

  begin
    result:=0;
    repeat
      v:=ReadB;
      result:=result shl 7;
      result:=result or (v and $7f);
    until (v and $80=0);
  end;

  function get24bitVal:longint;
  var
    ResultPTR:^Byte;
    // a,b,c:byte;

  begin
    ResultPTR:=@Result;
    getData(BI,3);
    ResultPTR^:=BI[2]; inc(ResultPTR);
    ResultPTR^:=BI[1]; inc(ResultPTR);
    ResultPTR^:=BI[0];
    // a:=ReadB;
    // b:=ReadB;
    // c:=ReadB;
    // ResultPTR^:=c;
    // inc(ResultPTR); ResultPTR^:=b;
    // inc(ResultPTR); ResultPTR^:=a;
  end;

begin
  trackData:=Track^.ptr;
  event:=Track^._event;
  repeat
    if not Track^.skipDelta then
    begin
      deltaTime:=getVarLong;
      if deltaTime>0 then
        break;
    end
    else
      Track^.skipDelta:=false;

    if TrackData^ and $80<>0 then
      event:=ReadB;

    case event of
      $80..$BF,
      $E0..$EF: // two parameters for event
        begin
{$IFDEF USE_FIFO}
          FIFO_WriteByte(event);
          FIFO_WriteByte(ReadB);
          FIFO_WriteByte(ReadB);
{$ELSE}
          MC6850_Send(event);
          MC6850_Send(ReadB);
          MC6850_Send(ReadB);
{$ENDIF}
        end;
      $C0..$DF: // one parameter for event
        begin
{$IFDEF USE_FIFO}
          FIFO_WriteByte(event);
          FIFO_WriteByte(ReadB);
{$ELSE}
          MC6850_Send(event);
          MC6850_Send(ReadB);
{$ENDIF}
        end;
      $F0..$F7: // SysEx Event
        begin
          msgLen:=getVarLong;
{$IFDEF USE_FIFO}
          FIFO_WriteByte(event);
{$ELSE}
          MC6850_Send(event);
{$ENDIF}
          while msgLen>0 do
          begin
            v:=ReadB;
{$IFDEF USE_FIFO}
            FIFO_WriteByte(v);
{$ELSE}
            MC6850_Send(v);
{$ENDIF}
            dec(msgLen);
          end;
          if v=$F7 then flagSysEx:=false else flagSysEx:=true;
        end;
      $FF: // Meta events
        begin
          meta:=ReadB;
          msgLen:=getVarLong;
          case meta of
            $2f: // end of track
              begin
                Track^.EOT:=true;
              end;
            $51: // tempo
              begin
                ms_per_qnote:=get24bitVal;
                setTempo(ms_per_qnote);
              end;
            $58: // time signature
              begin
                tactNum:=ReadB;
                tactDenum:=ReadB;
                ticks_per_qnote:=ReadB;
                ticks_per_32nd:=ReadB;
                setTempo(ms_per_qnote);
              end;
          else
            skip(msgLen);
          end;
        end;
    end;
  until Track^.EOT;
  Track^.ptr:=Pointer(TrackData);
  Track^.skipDelta:=true;
  Track^._event:=event;
  result:=deltaTime;
end;


procedure int_play; Interrupt;
begin
  if (_timerStatus and f_counter)=0 then
  begin
    inc(_subCnt);
    if _subCnt>=freq_ratio then
    begin
      _subCnt:=0;
      inc(totalTicks);
      _timerStatus:=_timerStatus or f_tick;
    end;
  end;
  asm
    pla
  end;
end;

procedure setTempo(nTempo:longint);
var
  freq:single;
  fdiv:byte;

begin
  setIntVec(iTim1,oldTimerVec);
  freq:=nTempo/1000000;
  freq:=freq/tickDiv;
  freq:=1/freq;
  // freq:=1/((tickDiv*ticks_per_qnote)/1000000)*8;
  fdiv:=round(64000/(freq*freq_ratio));
  BPM:=60000000 div nTempo;
  // writeLn(tickDiv,#127,nTempo,#127,ticks_per_qnote,#127,BPM,#127,round(freq));
  setIntVec(iTim1,@int_play,0,fdiv);
end;

initialization
  oldTimerVec:=nil;
  tickDiv:=384;
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
  ms_per_qnote:=500000;
  _timerStatus:=0;

  getIntVec(iTim1,oldTimerVec);
  // oldIRQ:=VIMIRQ;
end.