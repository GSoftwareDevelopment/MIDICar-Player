procedure createListEntry(t:byte; s:PString);
begin
  p_type:=t;
  list_putByte(p_type); list_putText(s); inc(lstTotal);
end;

procedure clearList;
begin
  listPtr:=pointer(LIST_ADDR);
  lstTotal:=0; lstCurrent:=0; lstY:=0; lstShift:=0;
  chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    putSpaces(32);
    inc(chn);
  end;
  fillchar(listPtr,$4000,$9B);
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

begin
  asm
    stx v     ; temporarly store X-Reg

    ldy #0

    lda MAIN.LIST.p_type
    bpl noCurrentPlay
    lda #$7B
    bpl putSign
  noCurrentPlay:
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
    lda #0
    iny
    sta (screen.scradr),y
    iny
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
var
  n:SmallInt;

begin
  IOResult:=0; list_go2Entry(lstShift); chn:=0; n:=lstShift;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    list_getByte(p_type);
    if n<>0 then
      if n=curPlay then p_type:=p_type or $80;
    list_getText(outstr);
    if IOResult=ERR_LIST_END then break;
    if IOResult=0 then list_getText(outstr);
    IOResult:=0;
    showEntry;
    inc(chn); inc(n);
  end;
  IOResult:=0;
end;

procedure choiceListFile;
begin
  list_go2Entry(lstCurrent);
  if IOResult=ERR_LIST_END then exit;

  list_getByte(p_type);
  list_getText(Snull);

  if stateInputLine<3 then
  begin
    if (p_type=fl_parentdir) then
    begin
      parentDir(curPath);
      resultInputLine:=True;
    end;
    if (p_type=fl_directory) then
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
    if (screenStatus and ss_minMode=0) then
    begin
      if p_type=fl_device then
      begin
        if screenStatus and ss_plsMode<>0 then
        begin
          outStr:=Snull;
          resultInputLine:=True;
        end
        else
        begin
          drawListSelection;
          ilscradr:=listScrAdr[lstY];
          init_inputLine;
        end;
      end;
    end;
  end;

  if (p_type=fl_playlist) or
      (p_type=fl_midifile) then
    if screenStatus and ss_plsMode=0 then
      joinStrings(outStr,Snull)
    else
      outStr:=Snull;

  if p_type=fl_midifile then
    IOResult:=loadSong;
end;
