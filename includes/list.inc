procedure drawListSelection;
begin
  scradr:=listScrAdr[YFile]; invers(32);
end;

procedure showEntry;
var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

begin
  getEntrySets(p_bank);
  v:=0;
//   if p_bank=$FD then v:=$5b; // playlist file
  if p_bank=fl_device then v:=$1a; // current path
  if p_bank=fl_parentdir then v:=$1c; // directory
  if p_bank=fl_directory then v:=$1e; // directory
  if p_bank=fl_midifile then v:=$5a; // song
  if p_bank<$40 then
    if p_adr=$00 then
      v:=$4d  // song not loaded
    else
      v:=$5a; // file loaded into memory

  poke(scradr,v); inc(scradr,2);
  v:=byte(outstr[0]);
  if v>LIST_VISIBLE_CHARS then outstr[0]:=char(LIST_VISIBLE_CHARS);
  putASCText(outstr);
  putSpaces(30-v);
end;

procedure showList;
begin
  gotoNEntry(shFile); chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    if not getEntry(outstr) then break;
    showEntry;
    inc(chn);
    nextEntry;
  end;
end;

procedure validPath;
begin
  _v:=destructureFullPath(outstr,curDev,curPath,fn);
//  if (_v and (dp_dev+dp_path)<>(dp_dev+dp_path)) then
  if (_v and dp_dev=0) then
  begin
    gotoNEntry(0); getEntry(outStr);
    _v:=destructureFullPath(outstr,curDev,curPath,Snull);
    if (_v and dp_dev=0) then curDev:='D:';
    if (_v and dp_path=0) then getCurrentPath(2,curDev,curPath);
  end;
  if byte(curPath[0])>0 then
    PathDelimiter(curPath,'>');

  joinStrings(curDev,curPath);
  joinStrings(outStr,fn);
end;

procedure choiceListFile;
begin
  if stateInputline<>0 then exit;

  gotoNEntry(curFile);
  if getEntry(outStr) then
  begin
    getEntrySets(p_bank);
    validPath;
    if (p_bank=fl_parentdir) then
    begin
      parentDir(curPath);
      joinStrings(curDev,curPath);
      resultInputLine:=True;
      stateInputLine:=2;
    end;
    if (p_bank=fl_directory) then
    begin
      PathDelimiter(outStr,'>');
      resultInputLine:=True;
      stateInputLine:=2;
    end;
    if p_bank=fl_midifile then
    begin
      statusStopped;
      IOResult:=loadSong;
      stateInputLine:=2;
    end;
    if p_bank=fl_device then
    begin
      drawListSelection;
      Snull:=outStr;
      ilscradr:=listScrAdr[YFile]+2;
      init_inputLine;
    end;
  end;
end;
