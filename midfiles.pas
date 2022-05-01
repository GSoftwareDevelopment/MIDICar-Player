unit MIDFiles;

interface
uses Objects;

const
  freq_ratio=2;

type
  TDeltaTime = longint;
  TByteArray = Array[0..0] of byte;
  TBigIndian = Array[0..3] of byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    // ptr:Pointer;
    pos:Cardinal;
    deltaTime:TDeltaTime;
    skipDelta,
    EOT:Boolean;
    _event:byte;
  end;

var
  // MIDData:TByteArray;
  MIDData:TMemoryStream;
  MIDTracks:TByteArray;
  format:word;
  nTracks:word;
  fps:byte;
  fsd:byte;
  tickDiv:word;
  totalTicks:longint;
  ms_per_qnote:longint;
  tactNum:byte;
  tactDenum:byte;
  ticks_per_qnote:byte;
  ticks_per_32nd:byte;
  BPM:Word;

  oldVec:Pointer;
  _pauseCount:boolean = false;
  _subCnt:byte;

function LoadMID(fn:PString):Boolean;
function GetTrackData(track:PMIDTrack):TDeltaTime;
procedure setTempo(nTempo:longint);

implementation
Uses
  {$IFDEF USE_FIFO}MIDI_FIFO,{$ENDIF}
  MC6850
  {$IFDEF DEBUG},CRT{$ENDIF}
  ;

type
  TTag = Longint;

const
  TAG_MTHD : TTag = $6468544D;
  TAG_MTRK : TTag = $6B72544D;

var
  BI:TBigIndian;
  RBuf:TByteArray absolute $600;

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

//
//
//

function LoadMID(fn:PString):boolean;
var
  f:File;
  trackCount:word;
  chunkTag:TTag;
  v,dataPos:word;
  Len:Longint;
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
  MIDData.Create;
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
  trackCount:=0; dataPos:=0; nTracks:=255;
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
      // BlockRead(f,MIDData[dataPos],Len);
      nTrkRec^.pos:=MIDData.Position;
      while Len>0 do
      begin
        if len>256 then v:=256 else v:=len;
        BlockRead(f,RBuf,v);
        MIDData.WriteBuffer(RBuf,v);
        Dec(Len,v);
      end;
      // nTrkRec^.ptr:=@MIDData[dataPos];
      nTrkRec^.deltaTime:=0;
      nTrkRec^.skipDelta:=false;
      nTrkRec^.EOT:=false;
      inc(nTrkRec,1);
      inc(dataPos,Len);
    end;
    Write(#156);
  end;
  Close(f);
  result:=true;
end;

function GetTrackData(track:PMIDTrack):TDeltaTime;
var
  TrkPos:Cardinal;
  // TrackData:^Byte;
  flagSysEx:Boolean;
  DeltaTime,msgLen:TDeltaTime;
  v,Event:Byte;

  function getVarLong:TDeltaTime;
  var
    v:byte;

  begin
    result:=0;
    repeat
      // v:=TrackData^; inc(TrackData);
      v:=MIDData.ReadByte;
      result:=result shl 7;
      result:=result or (v and $7f);
    until (v and $80=0);
  end;

  function getByte:Byte;
  begin
    // result:=TrackData^; inc(TrackData);
    result:=MIDData.ReadByte;
  end;

  function get24bitVal:longint;
  var
    ResultPTR:^Byte;
    a,b,c:byte;

  begin
    ResultPTR:=@Result;
    a:=MIDData.ReadByte;
    b:=MIDData.ReadByte;
    c:=MIDData.ReadByte;
    // a:=TrackData^; inc(TrackData);
    // b:=TrackData^; inc(TrackData);
    // c:=TrackData^; inc(TrackData);
    ResultPTR^:=c;
    inc(ResultPTR); ResultPTR^:=b;
    inc(ResultPTR); ResultPTR^:=a;
  end;

begin
  //TrackData:=Track^.ptr;
  MIDData.Position:=Track^.pos;
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

    v:=MIDData.readByte;
    if v and $80<>0 then
      event:=v
    else
      Dec(MIDData.Position);
    // if TrackData^ and $80<>0 then
    //   event:=getByte;

    case event of
      $80..$BF,
      $E0..$EF: // two parameters for event
        begin
{$IFDEF USE_FIFO}
          FIFO_WriteByte(event);
          FIFO_WriteByte(getByte);
          FIFO_WriteByte(getByte);
{$ELSE}
          MC6850_Send(event);
          MC6850_Send(getByte);
          MC6850_Send(getByte);
{$ENDIF}
        end;
      $C0..$DF: // one parameter for event
        begin
{$IFDEF USE_FIFO}
          FIFO_WriteByte(event);
          FIFO_WriteByte(getByte);
{$ELSE}
          MC6850_Send(event);
          MC6850_Send(getByte);
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
            v:=getByte;
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
          event:=getByte;
          msgLen:=getVarLong;
          case event of
            $2f: // end of track
              Track^.EOT:=true;
            $51: // tempo
              begin
                ms_per_qnote:=get24bitVal;
                setTempo(ms_per_qnote);
              end;
            $58: // time signature
              begin
                tactNum:=getByte;
                tactDenum:=getByte;
                ticks_per_qnote:=getByte;
                ticks_per_32nd:=getByte;
                // setTempo(ms_per_qnote);
              end;
          else
            // inc(TrackData,msgLen);
            inc(MIDData.Position,msgLen);
          end;
        end;
    end;
  until Track^.EOT;
  // Track^.ptr:=Pointer(TrackData);
  Track^.pos:=MIDData.Position;
  Track^.skipDelta:=true;
  Track^._event:=event;
  result:=deltaTime;
end;


procedure int_play; Interrupt;
begin
  if not _pauseCount then
  begin
    inc(_subCnt);
    if _subCnt>=freq_ratio then
    begin
      _subCnt:=0;
      inc(totalTicks);
    end;
  end;
  asm
    pla
  end;
end;

procedure setTempo(nTempo:longint);
var
  freq:float;
  _fdiv:byte;

begin
  setIntVec(iTim1,oldVec);

  freq:=1/((nTempo/tickDiv)/1000000)*freq_ratio;
  _fdiv:=round(64000/freq);
  BPM:=Round(60000000/nTempo);
  setIntVec(iTim1,@int_play,0,_fdiv);
end;

initialization
  oldVec:=nil;
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
  ms_per_qnote:=500000;

  getIntVec(iTim1,oldVec);
end.