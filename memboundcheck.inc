{
A procedure to check for memory area boundary crossing.

_Must be customized individually for the project._
}
procedure memBoundCheck; Assembler;
asm
  lda _adr
  beq check1Bound
  rts

check1Bound
  lda _adr+1
  cmp #$9c
  bne check2Bound

  lda #$a0
  sta _adr+1
  rts

check2Bound:
  cmp #$d0
  bne check3Bound

  lda #$d8
  sta _adr+1
  rts

check3Bound:
  cmp #$e0
  bne check4Bound

  lda #$e4
  sta _adr+1
  rts

check4bound
  cmp #$ff
  beq outOfMem
  rts

outOfMem:
  lda #2
  jmp @halt
end;

{
procedure memBoundCheck;
begin
  if _adr=$9c00 then
  begin
    // writeLn('gap $9c00-$a000');
    _adr:=$A000
  end
  else if _adr=$d000 then
  begin
    // writeLn('gap $d000-$d800');
    _adr:=$d800;
  end
  else if _adr=$E000 then
  begin
    // writeLn('gap $e000-$e400');
    _adr:=$e400;
  end
  else if _adr=$FF00 then
  begin
    WriteLn('Not enought memory.');
    halt(2);
  end;
end;
}
