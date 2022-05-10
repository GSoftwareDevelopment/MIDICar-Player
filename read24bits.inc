procedure read24; Assembler;
asm
  .local +MAIN.MIDFILES.PROCESSTRACK.READB
  m@INLINE
  .endl
  lda _TMP
  sta _delta+2

  .local +MAIN.MIDFILES.PROCESSTRACK.READB
  m@INLINE
  .endl
  lda _TMP
  sta _delta+1

  .local +MAIN.MIDFILES.PROCESSTRACK.READB
  m@INLINE
  .endl
  lda _TMP
  sta _delta+0

  lda #0
  sta _delta+3
end;
