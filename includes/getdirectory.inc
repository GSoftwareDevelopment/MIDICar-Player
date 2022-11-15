function getDirectory:Byte;
const
  CIO_CHN = 2;

var
  ext:DWord absolute $4ec;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+28; putINTText(#$4e+'Directory'~*+#$ce);

  statusStopped;
  statusReadDirectory;

  clearList; _adr:=$ffff; _bank:=fl_device; addToList(outStr);
  if byte(curPath[0])>1 then
  begin
    _adr:=$ffff; _bank:=fl_parentdir; addToList('..');
  end;
  joinStrings(outStr,dirMask);
  thi:=0;
  opn(CIO_CHN,6,0,outStr);
  while IOResult=1 do
  begin
    _bank:=fl_otherfile; // other file
    getLn(CIO_CHN,Snull);
    if Snull[1]>='0' then break;               // end of directory (first digit of free sectors)

    reduceFileName(Snull,fn);
    getFileExt(fn);

    if (Snull[2]=':') or                             // directory (SpartaDOS X >=4.47; MyDOS 4.53; Host device H:)
       (Snull[18]='>') then _bank:=fl_directory;     // directory (DOS XE 1.0)
    if (ext=FE_DIR) then                          // directory (SpartaDOS 3.2; SpartaDOS X <4.47; BW-DOS 1.30)
    begin
      fn[0]:=char(byte(fn[0])-4);
      _bank:=fl_directory;
    end;
    if ext=FE_MID then _bank:=fl_midifile;              // MIDI file
    // if ext=FE_PLS then _bank:=fl_playlist;              // playlist file

    addToList(fn);
  end;
  fn:='';
  clearStatus;
  result:=IOResult;
  cls(CIO_CHN);
  thi:=255;
end;

