procedure read24; Assembler;
asm
  .MACRO m@readByte
    ldy #0
    lda (_PTR),y
    sta _TMP
    inc _ADR
    sne
    inc _ADR+1
    jsr MEMBOUNDCHECK
    lda _TMP
  .ENDM

  m@readByte
  sta _delta+2

  m@readByte
  sta _delta+1

  m@readByte
  sta _delta+0

  lda #0
  sta _delta+3
end;
