function LoadMID(fn:PString):shortint;
const
  LOADBUF_SIZE=16*1024;
  CIO_CHN = 1;

var
{$IFNDEF USE_CIO}
  f:File;
{$ENDIF}
  trackCount:byte;
  chunkTag:TTag;
  v,top,endAdr,loadSize:Word;
  Len:Longint;
  trkData:PMIDTrack;
  RBuf:TByteArray absolute $4000;

{$i bigendian.inc}

  procedure _bget(dest:pointer);
  begin
{$IFNDEF USE_CIO}
    BlockRead(f,dest,loadSize,v);
{$ELSE}
    BGet(CIO_CHN,dest,loadSize);
{$ENDIF}
  end;

begin
// {$I-}
{$IFNDEF USE_CIO}
  Assign(f,fn);
  Reset(f,1);
{$ELSE}
  Opn(CIO_CHN,4,0,fn);
{$ENDIF}
  if IOResult>127 then
  begin
    result:=IOResult;
{$IFNDEF USE_CIO}
  Close(f);
{$ELSE}
  Cls(1);
{$ENDIF}
    exit;
  end;

  trkData:=@MIDTracks;
  trackCount:=0;
  totalTracks:=255;

  while (IOResult=1) {and (not EOF(F))} and (trackCount<totalTracks) do
  begin
{$IFNDEF USE_CIO}
    BlockRead(f,chunkTag,4,v);
    if (v<>4) then break;
{$ELSE}
    BGet(CIO_CHN,@chunkTag,4);
{$ENDIF}
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

      trkData^.status:=0;
      trkData^.trackTime:=0;
      trkData^.bank:=_bank;
      trkData^.ptr:=pointer(_adr);

      asm ldy _bank \ lda $100,y \ sta PORTB end;

      while Len>0 do
      begin
        loadProcess(trackCount);
        memBoundCheck;
        if IOResult<>1 then
        begin
          IOResult:=ERR_NOT_ENOUGHT_MEMORY;
          break;
        end;

        if hi(_adr)>=$d8 then loadSize:=$ff00
        else if hi(_adr)>=$c0 then loadSize:=$d000
        else if hi(_adr)>=$a8 then loadSize:=$c000
        else if hi(_adr)>=$40 then loadSize:=$8000;
        dec(loadSize,_adr);
        if loadSize=0 then continue;
        if loadSize>len then loadSize:=len;

        if hi(_adr)>=$c0 then
        begin
          _bget(@RBuf);
          move(RBuf,_ptr,loadSize);
        end
        else
          _bget(_ptr);

{$IFNDEF USE_CIO}
        if v<>loadSize then exit(IOResult);
{$ELSE}
        if IOResult<>1 then break;
{$ENDIF}

        inc(_adr,loadSize);
        Dec(Len,loadSize);
      end;

      inc(trkData,1);
    end;
  end;
{$IFNDEF USE_CIO}
  Close(f);
{$ELSE}
  Cls(1);
{$ENDIF}
  if IOResult<>1 then result:=0;
end;
