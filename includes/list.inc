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

// var
//   p_bank:byte;  //  \
//   p_adr:word;   //  / It is important that this order remains intact

begin
  getEntrySets(p_bank);
  asm
    lda MAIN.LIST.p_bank
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

    lda MAIN.FILESTR.OUTSTR_ADDR
    cmp #LIST_VISIBLE_CHARS
    bmi putString
    beq putString

// truncate String
    stx v     ; temporarly store X-Reg

    sub #LIST_VISIBLE_CHARS-2
    tax

    lda #$60
    ldy #2
    sta (screen.scradr),y

@:
    lda MAIN.FILESTR.OUTSTR_ADDR,x

    asl
    adc #$c0
    spl:eor #$40
    lsr
    scc:eor #$80

    iny
    sta (screen.scradr),y
    inx
    cpy #LIST_VISIBLE_CHARS+2
    bne @-

    ldx v       ; restore X-Reg

    rts

// put string on screen
putString:
    stx v     ; temporarly store X-Reg
    tax

    ldy #2
@:
    lda MAIN.FILESTR.OUTSTR_ADDR-1,y

    asl
    adc #$c0
    spl:eor #$40
    lsr
    scc:eor #$80

    sta (screen.scradr),y
    iny
    dex
    bne @-

    ldx v       ; restore X-Reg

// clear rest of line
    lda #0
    jmp checkClear
  clear:
    sta (screen.scradr),y
    iny
  checkClear:
    cpy #LIST_VISIBLE_CHARS+2
    bmi clear
    beq clear

  dontClear:
  end;
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
  gotoNEntry(lstCurrent);
  if getEntry(Snull) then
  begin
    getEntrySets(p_bank);
    if stateInputLine<3 then
    begin
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
    end;
    joinStrings(curDev,curPath);
    if stateInputLine<3 then
    begin
      stateInputLine:=ils_done;
      if p_bank=fl_device then
      begin
        drawListSelection;
        ilscradr:=listScrAdr[lstY]+2;
        init_inputLine;
      end;
    end;
    if p_bank=fl_midifile then
    begin
      joinStrings(outStr,Snull);
      IOResult:=loadSong;
    end;
  end;
end;
