procedure drawListSelection;
begin
  scradr:=listScrAdr[YFile]; invers(32);
end;

procedure showEntry;
const
  listchars : array[0..4] of byte = (
    $5a,  // fl_midifile
    $5b,  // fl_playlist
    $1c,  // fl_parentdir
    $1e,  // fl_directory
    $1a   // fl_device
  );

var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

begin
  getEntrySets(p_bank);
  asm
    lda p_bank
    sub #fl_midifile
    bcc nullChar
    tay
    lda adr.listchars,y
    jmp putChar

  nullChar:
    lda #0

  putChar:
    ldy #0
    sta (scradr),y
  end;
  inc(scradr,2);
  v:=byte(outstr[0]);
  if v>LIST_VISIBLE_CHARS then
  begin
    outstr[0]:=char(LIST_VISIBLE_CHARS);
    move(outstr[v-LIST_VISIBLE_CHARS+2],outstr[2],LIST_VISIBLE_CHARS);
    outstr[1]:=#$60;
    v:=LIST_VISIBLE_CHARS
  end;
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
