{
A procedure to check for memory area boundary crossing.

_Must be customized individually for the project._
}

{
  memory under XDOS 2.43N:

+----------------------------------------------------------------+
| $0700..$1df0 XDOS                                              |
+----------------------------+-----------------------------------+
| $4000..$7fff Free mem      | extended memory bank window       |
+----------------------------+-----------------------------------+
| with BASIC                 | without BASIC                     |
+----------------------------+-----------------------------------+
| $8000..$9c1f Free mem      | $8000..$bc1f Free mem             |
| $9c20..$9c3f Display List  | $bc20..$bc3f Display List         |
| $9c40..$9fff Screen        | $bc40..$bfff Screen               |
| $a000..$bfff Free mem      |                                   |
+----------------------------+-----------------------------------+
| RAM under ROM                                                  |
+----------------------------------------------------------------+
| $c000..$cfff Free mem                                          |
| $d000..$d7ff I/O Area                                          |
| $d800..$dfff Free mem                                          |
| $e000..$e3ff Font set                                          |
| $e400..$fffd Free mem                                          |
+----------------------------------------------------------------+

}

procedure memBoundCheck; Assembler;
asm
  lda _adr            // get LSB of address
  beq checkMemType    // zero means, we crossing the page. Check block bounds
  rts

checkMemType
  lda _adr+1          // get MSB of address
  ldy _bank           // get current bank index
  bne checkXMSBound   // zero, means we checking bounds in base memory
                      // otherwise, we checking bound of extended memory

// base memory bounds check
check1Bound           // 1st boundary $8000..$a3ff - program area
  cmp #$80
  bne check2Bound

  lda #$a4
  sta _adr+1
  rts

check2Bound:          // 2nd boundary $bc00..$bfff - DL $ Screen (BASIC OFF!)

  // cmp #$bc
  // bne check3Bound

  // lda #$c0
  // sta _adr+1
  // rts

check3Bound:          // 3rd boundary $d000..$d7ff - I/O area
  cmp #$d0
  bne check4Bound

  lda #$d8
  sta _adr+1
  rts

check4bound           // 4th boundary $ff00..$ffff
  cmp #$ff
  beq switchXMSBank
  rts

// extended memory bound check

checkXMSBound         // extender memory window $4000..$7fff
  cmp #$80            // the bank's memory area ends at $7fff
  beq switchXMSBank   // if MSB addres is out of this address, we need switch bank

  rts

switchXMSBank:
  iny             // increase current bank index
  lda $100,y      // get bank set
  beq outOfMem    // if zero, means Out Of Memory

  sty _bank       // store current bank index

  sta PORTB       // set PORTB (switch bank)

  lda #$40        // set MSB address to the begining of bank
  sta _adr+1
  rts

outOfMem:
  lda #ERR_NOT_ENOUGHT_MEMORY
  sta SYSTEM.IOResult
end;

{
procedure memBoundCheck;
begin
  if lo(_adr)=0 then exit;
  if _bank=0 then
  begin
    if hi(_adr)=$9c00 then
    begin
      _adr:=$A000; exit
    end
    else if hi(_adr)=$d000 then
    begin
      _adr:=$d800; exit;
    end
    else if hi(_adr)=$E000 then
    begin
      _adr:=$e400; exit;
    end;
  end;

  inc(_bank);
  bankSet:=peek($100+_bank);
  if bankSet<>$00 then
  begin
    PORTB:=bankSet;
    _adr:=$4000;
  end
  else
    IOResult:=ERR_NOT_ENOUGHT_MEMORY;
end;
}
