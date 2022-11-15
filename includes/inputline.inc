
procedure show_inputLine; assembler;
asm
illen = 30

  txa:pha

; set screen row offset
  ldy #0

; get cursor position
  lda ilpos
  sta OUTSTR_ADDR
  beq cursor    ; if zero, draw cursor & clear inputline

; set outStr offset

  ldx #1

  cmp #illen
  bmi strLoop   ; if cursor position more than inputline length...

; set starting point of input line content
setOffset:
  sub #illen-2
  tax

; show input line content (outStr) on screen
strLoop:
  lda OUTSTR_ADDR,x

  asl
  adc #$c0
  spl:eor #$40
  lsr
  scc:eor #$80

  sta (ilscradr),y
  iny
  inx
  cpx ilpos
  bmi strLoop
  beq strLoop

cursor:
; check input line state
  lda stateInputLine
  cmp #ils_inprogress
  bne skipCursor

; draw cursor
  lda (ilscradr),y
  bmi nospace
  lda #0
noSpace:
  eor #$80
  sta (ilscradr),y
  iny

skipCursor:

; clear rest of inputline
  lda #0
  jmp checkSpaceLoop
spaceLoop:
  sta (ilscradr),y
  iny
checkSpaceLoop:
  cpy #illen
  bmi spaceLoop

  pla:tax
end;

procedure init_inputLine;
begin
  ilpos:=length(outstr);
  fillchar(outstr[ilpos+1],80-ilpos,$9b);
  keyb:=255;
  show_inputLine; ctm:=_tm;
  stateInputLine:=ils_inprogress;
  resultInputLine:=false;
end;

procedure do_inputLine;
begin
  if (keyb=k_ESC) or (keyb=k_RETURN) then
  begin
    if (keyb=k_ESC) or (ilpos=0) then outStr:=Snull;
    ilpos:=byte(outstr[0]);
    stateInputLine:=ils_done*byte(keyb=K_RETURN);
    show_inputLine;
    drawListSelection;
    resultInputLine:=(keyb=K_RETURN) and (ilpos>0);
    exit;
  end;
  if (ilpos>0) and (keyb=k_delete) then
  begin
    outstr[ilpos]:=#$9B;
    dec(ilpos);
  end;
  if ilpos<79 then
  begin
    ilch:=keyscan2asc(keyb);
    if ilch<>0 then
    begin
      inc(ilpos);
      outstr[ilpos]:=char(ilch);
    end;
  end;
  show_inputLine;
end;