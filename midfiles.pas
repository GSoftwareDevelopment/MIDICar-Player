unit MIDFiles;

interface
type
  TByteArray = Array[0..0] of byte;
  TBigIndian = Array[0..3] of byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    ptr:Pointer;
    deltaTime,
    size:Longint;
    skipDelta,
    EOT:Boolean;
  end;

var
  MIDData:TByteArray;
  MIDTracks:TByteArray;
  format:word;
  nTracks:word;
  fps:byte;
  fsd:byte;
  tickDiv:word;

Procedure LoadMID(fn:String);
Procedure GetTrackData(track:PMIDTrack);

implementation
Uses MIDI_FIFO
{$IFDEF DEBUG}
,CRT
{$ENDIF}
;

var
  BI:TBigIndian;

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

procedure LoadMID(fn:String);
var
  f:File;
  trackCount:word;
  chunkHead:String[4];
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
  nTrkRec:=@MIDTracks;
  Write('Open file ',fn);
  Assign(f,fn);
  Reset(f,1);
  if IOResult>127 then
  begin
    WriteLn(#155,'I/O Error #',IOResult);
    Close(f);
    exit;
  end;
  trackCount:=0; dataPos:=0; nTracks:=255;
  SetLength(chunkHead,4);
  while (IOResult<128) and (not EOF(F)) and (trackCount<nTracks) do
  begin
    BlockRead(f,chunkHead[1],4,v);
    if (v<>4) then break;
    Len:=ReadLongBI;
{$IFDEF DEBUG} Write('-',ChunkHead,'(',Len,') ');{$ENDIF}
    if chunkHead='MThd' then
    begin
      format:=readWordBI;
      nTracks:=readWordBI;
      v:=readWordBI;
{$IFDEF DEBUG}
      WriteLn('Format: ',format);
      WriteLn('Tracks: ',nTracks);
{$ENDIF}
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
    else if chunkHead='MTrk' then
    begin
      inc(trackCount);
{$IFDEF DEBUG} Write('Track: ',trackCount,'/',nTracks,'...'); {$ELSE} Write('.'); {$ENDIF}
      BlockRead(f,MIDData[dataPos],Len);
      nTrkRec^.ptr:=@MIDData[dataPos];
      nTrkRec^.deltaTime:=0;
      nTrkRec^.size:=Len;
      nTrkRec^.skipDelta:=false;
      nTrkRec^.EOT:=false;
      inc(nTrkRec,1);
      inc(dataPos,Len);
    end;
{$IFDEF DEBUG} Write(#156); {$ENDIF}
  end;
  Close(f);
end;

procedure GetTrackData(track:PMIDTrack);
var
  TrackData:^Byte;
  flagSysEx:Boolean;
  DeltaTime,msgLen:Longint;
  v,Event:Byte;

  function decodeDeltaTime:longint;
  var
    v:byte;

  begin
    result:=0;
    repeat
      v:=TrackData^; inc(TrackData);
      result:=result shl 7+(v and $7f);
    until (v and $80=0);
  end;

  function getByte:Byte;
  begin
    result:=TrackData^; inc(TrackData);
  end;

begin
  TrackData:=Track^.ptr;
  repeat
    if not Track^.skipDelta then
    begin
      deltaTime:=decodeDeltaTime;
      if deltaTime>0 then
        break;
    end
    else
      Track^.skipDelta:=false;

    if TrackData^ and $80<>0 then
    begin
      event:=getByte;
      if (event>=$80) and (event<=$f7) then
        FIFO_WriteByte(event);
    end;

    case event of
      $C0..$DF: // one parameter for event
        begin
          FIFO_WriteByte(getByte);
        end;
      $80..$BF: // two parameters for event
        begin
          FIFO_WriteByte(getByte);
          FIFO_WriteByte(getByte);
        end;
      $E0..$EF: // three parameters for event
        begin
          FIFO_WriteByte(getByte);
          FIFO_WriteByte(getByte);
          FIFO_WriteByte(getByte);
        end;
      $F0..$F7: // SysEx Event
        begin
          msgLen:=decodeDeltaTime;
          while msgLen>0 do
          begin
            v:=getByte;
            FIFO_WriteByte(v);
            dec(msgLen);
          end;
          if v=$F7 then flagSysEx:=false else flagSysEx:=true;
        end;
      $FF: // Meta events
        begin
          event:=getByte;
          msgLen:=decodeDeltaTime;
          if event=$2f then
            Track^.EOT:=true
          else
          begin
            while msgLen>0 do
            begin
              v:=getByte; dec(msgLen);
            end;
          end;
        end;
    end;
  until Track^.EOT;
  Track^.ptr:=Pointer(TrackData);
  Track^.skipDelta:=true;
  Track^.deltaTime:=deltaTime;
end;

end.