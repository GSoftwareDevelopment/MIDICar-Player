unit MIDFiles;

interface
type
  TByteArray = Array[0..0] of byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    ptr:Pointer;
    deltaTime:Longint;
  end;

var
  MIDData:TByteArray;
  MIDTracks:TByteArray;
  format:word;
  nTracks:word;
  fps:byte;
  fsd:byte;
  timing:word;

Procedure LoadMID(fn:String);

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

begin
  nTrkRec:=@MIDTracks;
{$IFDEF DEBUG}
  WriteLn('Open file ',fn);
{$ENDIF}
  Assign(f,fn); Reset(f,1);
  trackCount:=0; dataPos:=0; nTracks:=255;
  SetLength(chunkHead,4);
  while (not EOF(F)) and (trackCount<nTracks) do
  begin
    BlockRead(f,chunkHead[1],4,v);
    if (v<>4) then break;
    Len:=ReadLongBI(f);
{$IFDEF DEBUG} Write('-',ChunkHead,'(',Len,') ');{$ENDIF}
    if chunkHead='MThd' then
    begin
      format:=readWordBI(f);
      nTracks:=readWordBI(f);
      v:=readWordBI(f);
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
{$IFDEF DEBUG}
        WriteLn('FPS: ',FPS);
{$ENDIF}
      end
      else
      begin
{$IFDEF DEBUG}
        WriteLn('TickDiv: ',v);
{$ENDIF}
      end;
    end
    else if chunkHead='MTrk' then
    begin
      inc(trackCount);
{$IFDEF DEBUG}
      WriteLn('Track: ',trackCount,'/',nTracks);
      WriteLn('Size: ',Len);
{$ENDIF}
      BlockRead(f,MIDData[dataPos],Len);
      nTrkRec^.ptr:=@MIDData[dataPos];
      nTrkRec^.deltaTime:=0;
      inc(nTrkRec,1);
      inc(dataPos,Len);
    end;
  end;
  Close(f);
end;

end.