function loadPlaylist(PLSFilename:PString):Byte;
const
  CIO_CHN = 1;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+30; putINTText(#$4e+'Playlist'~*+#$ce);

  gotoNEntry(0); getEntry(fn);
  clearPlaylist;
  _adr:=$ffff; _bank:=fl_device; addToPlaylist(fn);
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
