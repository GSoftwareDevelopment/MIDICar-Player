unit MIDFiles;

interface
const
  MIDTRACKSREC_ADDR = $5F00;
  MIDDATA_ADDR = $6000;

type
  TMIDTrack = record
    ptr:Pointer;
    deltaTime:Longint;
  end;

  TMIDHeader = record
    format:word;
    nTracks:word;
    fps:byte;
    fsd:byte;
    timing:word;
  end;

var
  MID_DATA:Array[0..0] of byte absolute MIDDATA_ADDR;
  Tracks:Array[0..0] of byte absolute MIDTRACKSREC_ADDR;
  CurTrack:TMIDTrack;
  MIDHeader:TMIDHeader;

procedure LoadMIDI(fn:String);

implementation
{$IFDEF DEBUG}
Uses CRT;
{$ENDIF}

var
  BI:Array[0..3] of byte;

function ReadWordBI(var f:File):word;
var
  ResultPTR:^Byte;

begin
  blockRead(f,BI,2);
  ResultPTR:=@Result;
  ResultPTR^:=BI[1]; inc(ResultPTR);
  ResultPTR^:=BI[0];
end;

function ReadLongBI(var f:File):longint;
var
  ResultPTR:^Byte;
  i:byte;

begin
  blockRead(f,BI,4);
  ResultPTR:=@Result;
  for i:=3 downto 0 do
  begin
    ResultPTR^:=BI[i];
    inc(ResultPTR);
  end;
end;

procedure LoadMIDI(fn:String);
var
  f:File;
  Ofs:^Byte;
  curTrack:byte;
  trackCount:byte;
  chunkHead:String[4];
  v,dataPos:word;
  Len:Longint;

begin
  Ofs:=@MID_DATA;
{$IFDEF DEBUG}
  WriteLn('Open file ',fn);
{$ENDIF}
  Assign(f,fn); Reset(f,1);
  curTrack:=0; trackCount:=255; dataPos:=0;
  SetLength(chunkHead,4);
  while (not EOF(F)) and (curTrack<=trackCount) do
  begin
    BlockRead(f,chunkHead[1],4,v);
    if (v<>4) then break;
    Len:=ReadLongBI(f);
{$IFDEF DEBUG}
  WriteLn('Chunk type: ',ChunkHead);
  WriteLn('Chunk size: ',Len);
{$ENDIF}
    if chunkHead='MThd' then
    begin
      MIDHeader.format:=readWordBI(f);
      MIDHeader.nTracks:=readWordBI(f);
      v:=readWordBI(f);
      if (v and $8000)<>0 then
      begin
        MIDHeader.fps:=(v shr 8) and $7F;
        case MIDHeader.fps of
          $E8 : MIDHeader.fps:=24;
          $E7 : MIDHeader.fps:=25;
          $E3 : MIDHeader.fps:=29;
          $E2 : MIDHeader.fps:=30;
        end;
      end;
    end
    else if chunkHead='MTrk' then
    begin
      BlockRead(f,MID_DATA[dataPos],Len);
      inc(dataPos,Len);
    end;
  end;
  Close(f);
end;

end.