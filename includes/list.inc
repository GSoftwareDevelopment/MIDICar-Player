procedure clearList;
begin
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
    $4c,  // fl_parentdir
    $4f,  // fl_directory
    $1a   // fl_device
  );

// var
//   p_bank:byte;  //  \
//   p_adr:word;   //  / It is important that this order remains intact

begin
  getEntrySets(p_bank);
  asm
    stx v     ; temporarly store X-Reg

    ldy #0

    lda MAIN.LIST.p_bank
    sub #fl_midifile
    bcc nullSign
    cmp #4
    beq noSign
    tax
    lda adr.listchars,x
    jmp putSign

  nullSign:
    lda #0

  putSign:
    sta (screen.scradr),y
    ldy #2    ; set screen offset
  noSign:
    ldx #0    ; set string offset

    lda MAIN.FILESTR.OUTSTR_ADDR  ; string length
    cmp #LIST_VISIBLE_CHARS       ; if string length is...
    bmi putString                 ; ...less or...
    beq putString                 ; ...equal...
                                  ; ...then, show whole string

// truncate String
    sub #LIST_VISIBLE_CHARS+1
    tax

    lda #$60                      ; put ellipsis character
    sta (screen.scradr),y
    iny                           ; increase screen offset


// put string on screen
putString:
@:
    lda MAIN.FILESTR.OUTSTR_ADDR+1,x

    asl
    adc #$c0
    spl:eor #$40
    lsr
    scc:eor #$80

    sta (screen.scradr),y
    iny
    inx
    cpx MAIN.FILESTR.OUTSTR_ADDR
    bmi putString

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
        ilscradr:=listScrAdr[lstY];
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
