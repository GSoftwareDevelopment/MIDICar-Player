procedure fileTypeByExt(s:PString);
var
  c0:byte absolute FN_PATH_ADDR;

begin
  getFileExt(s);
  if (fnExt=FE_DIR) then                          // directory (SpartaDOS 3.2; SpartaDOS X <4.47; BW-DOS 1.30)
  begin
    dec(c0,4);
    p_type:=fl_directory;
  end;
  if fnExt=FE_MID then p_type:=fl_midifile;       // MIDI file
  if fnExt=FE_PLS then p_type:=fl_playlist;       // playlist file
end;

function getDirectory:Byte;
const
  CIO_CHN = 1;

var
  desFiles:SmallInt;
  c1:char absolute SNULL_ADDR+1;
  c2:char absolute SNULL_ADDR+2;
  c18:char absolute SNULL_ADDR+18;

  procedure checkFileInList;
  var
    oIORes:Byte;
    repFind:Byte;

  begin
    oIORes:=IOResult;
    repFind:=2;
    while repFind>0 do
    begin
      IOResult:=0;
      while IOResult<>ERR_LIST_END do
      begin
        IOResult:=0;
        list_getByte(_v);
        list_GetText(Snull);
        if IOResult<>ERR_LIST_ENTRY_END then list_nextEntry;
        if IOResult<>ERR_LIST_END then
          if (_v=p_type) then
            if (Snull=fn) then
            begin
              IOResult:=ERR_LIST_FOUND; break;
            end;
      end;
      if IOResult=ERR_LIST_FOUND then break;
      if repFind=2 then listPtr:=pointer(LIST_ADDR);
      dec(repFind);
    end;
    _v:=IOResult;
    IOResult:=oIORes;
  end;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+28; putINTText(#$4e+'Directory'~*+#$ce);

  statusStopped;
  validPath;
  clearList;

  joinStrings(curDev,curPath);

  createListEntry(fl_device,outStr);
  if byte(curPath[0])>1 then
  begin
    createListEntry(fl_parentDir,'..');
    desFiles:=2;
  end
  else
    desFiles:=1;

  thi:=0;
  if keymod=0 then
  begin
    putStatus(statusMsgs[7]);
    IOResult:=readDescriptionFile(descriptionFile);
  end
  else
    IOResult:=ERR_FILE_NOT_FOUND;

  if IOResult=ERR_FILE_NOT_FOUND then
  begin
    desFiles:=lstTotal-desFiles;
    putStatus(statusMsgs[6]);

    joinStrings(curDev,curPath);
    joinStrings(outStr,dirMask);

    opn(CIO_CHN,6,0,outStr);
    while IOResult=1 do
    begin
      p_type:=fl_otherfile; // default other file
      getLn(CIO_CHN,Snull);
      if c1>='0' then break;                  // end of directory (first digit of free sectors)

      if (c2=':') or                          // directory (SpartaDOS X >=4.47; MyDOS 4.53; Host device H:)
         (c18='>') then p_type:=fl_directory; // directory (DOS XE 1.0)

      reduceFileName(Snull,fn);
      fileTypeByExt(fn);

      if (fnExt=FE_ION) then continue;                // skip DESCRIPT.ION file

      if desFiles>0 then
      begin
        checkFileInList;
        if _v=ERR_LIST_FOUND then
        begin
          dec(desFiles); continue;
        end;
      end;

      createListEntry(p_type,fn);
    end;
    result:=IOResult;
    cls(CIO_CHN);
  end;
  fn:='';
  clearStatus;
  thi:=255;
  screenStatus:=screenStatus or ss_isRefresh;
end;

