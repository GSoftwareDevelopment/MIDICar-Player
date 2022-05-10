{
A procedure to check for memory area boundary crossing.

_Must be customized individually for the project._
}

{
  memory under XDOS 2.43N:

  $0700..$1df0 XDOS
  $4000..$7fff Free mem - extended memory bank
  $8000..$9c1f Free mem
  $9c20..$9c3f Display List
  $9c40..$9fff Screen
  $a000..$bfff Free mem
  $c000..$d7ff Free mem (under ROM)
  $d800..$dfff Free mem (under ROM)
  $e000..$e3ff Font set
  $e400..$fffd Free mem (under ROM)

  free memory gaps in page align:

  $4000..$9bff
  $a000..$bfff
  $c000..$d7ff
  $e400..$ff00

  total free: AF00 (44800)
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
  lda #ERR_NOT_ENOUGHT_MEMORY
  sta SYSTEM.IOResult
  // jmp @halt
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
