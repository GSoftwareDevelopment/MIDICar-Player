function loadPlaylist(PLSFilename:PString):Byte;
const
  CIO_CHN = 1;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+30; putINTText(#$4e+'Playlist'~*+#$ce);

  gotoNEntry(0); getEntry(fn);
  clearPlaylist;
  _adr:=$ffff; _bank:=$fe; addToPlaylist(fn);
  fn:=PLSFilename; clearStatus;
  statusReadPlaylist;

  opn(CIO_CHN,4,0,PLSFilename);
  _bank:=0; _adr:=0;
  while IOResult=1 do
  begin
    getLn(CIO_CHN,outStr);
    dec(outStr[0]);
    if outStr[0]=#0 then continue;
    addToPlaylist(outStr);
  end;
  result:=IOResult;
  cls(CIO_CHN);
end;

procedure showPlaylist;
begin
  chn:=0;
  while (chn<16) do
  begin
    scradr:=playlistScrAdr[chn];
    putSpaces(PLAYLIST_VISIBLE_CHARS+2);
    inc(scradr);
    if getEntry(fn) then
    begin
      if byte(fn[0])>PLAYLIST_VISIBLE_CHARS then fn[0]:=char(PLAYLIST_VISIBLE_CHARS);
      putASCText(fn)
    end
    else
      break;
    inc(chn);
    nextEntry;
  end;
end;

procedure choicePlaylistFile;
var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact
  isEntry:boolean;
  dev:TDevString;

begin
  statusStopped;
  gotoNEntry(curFile);
  if not getEntry(fn) then exit;
  getEntrySets(@p_bank);

  getDeviceSpec(fn,dev);
  if dev='' then
  begin
    dev:=curdev; joinStrings(dev,fn);
  end
  else
    outstr:=fn;

  if p_bank=$fd then IOResult:=loadPlaylist(outStr);
  if p_bank=$fe then
  begin
    scradr:=playlistScrAdr[YFile];
    asm
      lda firstTime
      bne goFileSelector
    end;
    inputline(outstr);
    asm
      lda MAIN.INPUTLINE.Result
      bne goFileSelector
      jmp skip
    goFileSelector:
    end;
    getDeviceSpec(outstr,dev);
    if dev<>'' then curDev:=dev;
    IOResult:=fileSelect(outStr);
    firstTime:=false;
    asm
      skip:
    end;
    fn:=outstr;
  end;
  if p_bank=$ff then
  begin
    _bank:=last_bank;
    _adr:=last_adr;
    IOResult:=loadSong(outStr);
  end;
  keyb:=255;

  clearStatus;
  gotoNEntry(shFile);
  showPlaylist;
  drawPlaylistSelection;
  if IOResult and %11111100<>0 then statusError(IOResult);

  // if totalTracks<>0 then
  // begin
  //   last_bank:=_bank;
  //   last_adr:=adr;
  // end;
end;