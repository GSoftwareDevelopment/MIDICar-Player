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
  if (_v and dp_dev=0) then curDev:='D:';
  if (_v and dp_path=0) then getCurrentPath(2,curDev,curPath);
  PathDelimiter(curPath,'>');
end;

procedure choiceListFile;
begin
  if stateInputline<>ils_pending then exit;

  gotoNEntry(curFile);
  if getEntry(Snull) then
  begin
    getEntrySets(p_bank);
    if (p_bank=fl_parentdir) then
    begin
      parentDir(curPath);
      resultInputLine:=True;
    end;
    if (p_bank=fl_directory) then
    begin
      joinStrings(curPath,Snull);
      PathDelimiter(outStr,'>');
      curPath:=outStr;
      resultInputLine:=True;
    end;
    joinStrings(curDev,curPath);
    if p_bank=fl_midifile then
    begin
      statusStopped;
      joinStrings(outStr,Snull);
      validPath;
      IOResult:=loadSong;
    end;
    stateInputLine:=ils_done;
    if p_bank=fl_device then
    begin
      drawListSelection;
      ilscradr:=listScrAdr[YFile]+2;
      init_inputLine;
    end;
  end;
end;
