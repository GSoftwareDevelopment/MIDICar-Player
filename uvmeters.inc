hlpptr = $D6
  lda playerStatus
  and #ps_view
  bne updateUVMeter
  rts

updateUVMeter:
  txa:pha

  ldx #0
uvmeterLoop:
  lda MIDFILES.adr.CHNVOLUME,x
  beq dontUpdate

  cmp #4
  bpl decVol
  lda #0
  beq storeVol
decVol:
  sub #4
storeVol:
  sta MIDFILES.adr.CHNVOLUME,x

  lsr @
  and #%00111100

  add #<UVMETER_ADDR
  sta hlpptr
  lda #>UVMETER_ADDR
  sta hlpptr+1

  txa
  asl @
  tay
  lda adr.CHANNELSCRADR,y
  sta scradr
  lda adr.CHANNELSCRADR+1,y
  sta scradr+1

  ldy #3
loop:
  lda (hlpptr),y
  sta (scradr),y
  dey
  bpl loop

dontUpdate:
  inx
  cpx #16
  bne uvmeterLoop

  pla:tax