procedure clearList;
begin
  resetMemoryPtr;
  fileList:=pointer(LIST_ADDR);
  lstTotal:=0; lstCurrent:=0; lstY:=0; lstShift:=0;
  chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    putSpaces(32);
    inc(chn);
  end;
  fillchar(fileList,$1000{$2700},$9B);
end;

procedure drawListSelection;
begin
  scradr:=listScrAdr[lstY]; invers(32);
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
    sta (screen.scradr),y
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
  gotoNEntry(lstShift); chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    if not getEntry(outstr) then break;
    showEntry;
    inc(chn);
    nextEntry;
  end;
end;

procedure choiceListFile;
begin
  if stateInputline<>ils_pending then exit;

  gotoNEntry(lstCurrent);
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
      joinStrings(outStr,Snull);
      IOResult:=loadSong;
    end;
    stateInputLine:=ils_done;
    if p_bank=fl_device then
    begin
      drawListSelection;
      ilscradr:=listScrAdr[lstY]+2;
      init_inputLine;
    end;
  end;
end;
