  .MACRO m@readByte storeTo
    ldy #0
    lda (_PTR),y
    sta :storeTo
    inc _ADR
    sne
    inc _ADR+1
    jsr MEMBOUNDCHECK
  .ENDM

  m@readByte _delta+2
  m@readByte _delta+1
  m@readByte _delta+0
  lda #0
  sta _delta+3
