function fileSelect(path:PString):Byte;
const
  CIO_CHN = 2;

var
  dentry:string[20];
  ext:DWord;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+28; putINTText(#$4e+'Directory'~*+#$ce);

  clearList;
  _adr:=$ffff; _bank:=$fe; addToList(path);
  statusReadDirectory;

  opn(CIO_CHN,6,0,path);
  while IOResult=1 do
  begin
    _bank:=$fc; // other file
    getLn(CIO_CHN,dentry);
    if dentry[1]>='0' then break;               // end of directory (first digit of free sectors)
    reduceFileName(dentry,outstr);
    ext:=getFileExt(outstr);
    if dentry[2]=':' then continue;             // directory (SpartaDOS X >=4.47; MyDOS 4.53)
    if dentry[18]='>' then continue;            // directory (DOS XE 1.0)
    if ext=FE_DIR then continue;                // directory (SpartaDOS 3.2; SpartaDOS X <4.47; BW-DOS 1.30)
    if ext=FE_MID then _bank:=$ff;              // MIDI file
    // if ext=FE_PLS then _bank:=$fd;              // playlist file

    addToList(outstr);
  end;
  result:=IOResult;
  cls(CIO_CHN);
end;

