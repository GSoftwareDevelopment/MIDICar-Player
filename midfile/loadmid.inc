function LoadMID:shortint;
const
  LOADBUF_SIZE=16*1024;
  CIO_CHN = 1;

var
  fn:String[80] absolute $55a;
  trackCount:byte absolute $de;
  chunkTag:TTag;
  loadSize:Word absolute $d6;
  Len:Longint; // absolute $e9;
  trkData:PMIDTrack;
  RBuf:TByteArray absolute $4000;

{$i bigendian.inc}

  procedure _bget(dest:pointer);
  begin
    BGet(CIO_CHN,dest,loadSize);
  end;

  procedure checkLoadSize;
  begin
    loadSize:=0;
    repeat
      memBoundCheck;
      if IOResult<>1 then exit;

// Since, ONLY extended memory is used, there is no need to check ranges
      { if hi(_adr)>=$d8 then loadSize:=$ff00
      else if hi(_adr)>=$c0 then loadSize:=$d000
      else if hi(_adr)>=$a8 then loadSize:=$c000
      else } if hi(_adr)>=$40 then loadSize:=$8000;
      dec(loadSize,_adr);
      if loadSize>len then loadSize:=len;
    until loadSize<>0;
  end;

begin
  Opn(CIO_CHN,4,0,fn);
  if IOResult>127 then
  begin
    result:=IOResult;
    Cls(1);
    exit;
  end;

  us_per_qnote:=500000;
{$IFDEF USE_SUPPORT_VARS}
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
  BPM:=120;
{$ENDIF}

  trkData:=MIDTracks;
  trackCount:=0;
  totalTracks:=255;
  _songTicks:=0;

  while (IOResult=1) and (trackCount<totalTracks) do
  begin
    BGet(CIO_CHN,@chunkTag,4);
    Len:=ReadLongBI;
    if chunkTag=TAG_MTHD then
    begin
      format:=readWordBI;
      if (format<>MID_0) and (format<>MID_1) then
      begin
        IOResult:=ERR_UNSUPPORTED_FORMAT;
        break;
      end;
      totalTracks:=readWordBI;
      tickDiv:=readWordBI;
{$IFDEF USE_SUPPORT_VARS}
      if (tickDiv and $8000)<>0 then
      begin
        fps:=(hi(tickDiv) xor 255)+1;
        fsd:=lo(tickDiv);
      end;
{$ENDIF}
      loadProcess(0);
    end
    else if chunkTag=TAG_MTRK then
    begin
      inc(trackCount);

      trkData^.trackTime:=0;
      trkData^._eStatusRun:=0;
      trkData^.bank:=_bank;
      trkData^.ptr:=pointer(_adr);

      asm ldy _bank \ lda $100,y \ sta PORTB end;

      while Len>0 do
      begin
        checkLoadSize;
        loadProcess(trackCount);
        if IOResult<>1 then break;
// Since, ONLY extended memory is used, there is no need to check ranges
{        if hi(_adr)>=$c0 then
        begin
          _bget(@RBuf);
          move(RBuf,_ptr,loadSize);
        end
        else}
          _bget(_ptr);

        if IOResult<>1 then break;

        inc(_adr,loadSize);
        Dec(Len,loadSize);
      end;

      _bank:=trkData^.bank;
      _ptr:=trkData^.ptr;
      _trackTime:=0;
      _eStatusRun:=0;
      calcTrackLength;
      if _trackTime>_songTicks then _songTicks:=_trackTime;

      inc(trkData,1);
    end else
      IOResult:=ERR_NOT_A_MIDI_FILE;
  end;
  _songTicks:=_songTicks div 160;
  asm lda #$FE \ sta PORTB end;
  result:=IOResult;
  Cls(1);
end;
