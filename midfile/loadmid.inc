function LoadMID(fn:PString):shortint;
const
  LOADBUF_SIZE=255;

var
  f:File;
  trackCount:byte;
  chunkTag:TTag;
  v,top,endAdr:Word;
  Len:Longint;
  loadSize:Byte;
  trkData:PMIDTrack;

{$i bigindian.inc}

begin
{$I-}
  Assign(f,fn);
  Reset(f,1);
  if IOResult>127 then
  begin
    result:=IOResult;
    Close(f);
    exit;
  end;

  _bank:=0;
  _ptr:=MIDData;
  trkData:=@MIDTracks;
  trackCount:=0;
  totalTracks:=255;

  while (IOResult=1) and (not EOF(F)) and (trackCount<totalTracks) do
  begin
    BlockRead(f,chunkTag,4,v);
    if (v<>4) then break;
    Len:=ReadLongBI;
    if chunkTag=TAG_MTHD then
    begin
      format:=readWordBI;
      if (format<>MID_0) and (format<>MID_1) then exit(ERR_UNSUPPORTED_FORMAT);
      totalTracks:=readWordBI;
      v:=readWordBI;
      if (v and $8000)=0 then
        tickDiv:=v
{$IFDEF USE_SUPPORT_VARS}
      else
      begin
        fps:=(v shr 8) and $7F;
        case fps of
          $E8 : fps:=24;
          $E7 : fps:=25;
          $E3 : fps:=29;
          $E2 : fps:=30;
        end;
        fsd:=v and $ff;
      end
{$ENDIF}
    end
    else if chunkTag=TAG_MTRK then
    begin
      inc(trackCount);
      loadProcess(trackCount);

      trkData^.bank:=_bank;
      trkData^.ptr:=pointer(_adr);
      trkData^.trackTime:=0;
      trkData^.status:=0;

      while Len>0 do
      begin
        memBoundCheck; if IOResult<>1 then exit(ERR_NOT_ENOUGHT_MEMORY);

        if len>LOADBUF_SIZE then loadSize:=LOADBUF_SIZE else loadSize:=len;
        endAdr:=_adr+loadSize;
        if _bank=0 then
        begin
          if (hi(endAdr)>=$80) and (hi(endAdr)<$a4) then
            loadSize:=$8000-_adr
          else if (hi(endAdr)>=$d0) and (hi(endAdr)<$d8) then
            loadSize:=$d000-_adr
          else if (hi(endAdr)>=$ff) then
            loadSize:=$ff00-_adr
        end
        else
          if (hi(endAdr)>=$80) then
            loadSize:=$8000-_adr;

        if loadSize=0 then continue;

        BlockRead(f,RBuf,loadSize,v);
        if v<>loadSize then exit(IOResult);

        move(@RBuf,_ptr,loadSize);
        inc(_adr,v);
        Dec(Len,v);
      end;

      inc(trkData,1);
    end;
  end;
  Close(f);
  result:=0;
end;
