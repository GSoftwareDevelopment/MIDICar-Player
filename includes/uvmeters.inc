// hlpptr = $D6

  lda playerStatus
  and #ps_view
  bne updateUVMeter
  rts

updateUVMeter:
  stx regx // txa:pha

  ldx #0
uvmeterLoop:
  lda MIDFILES.adr.CHNVOLUME,x
  beq dontUpdate

  sub #4
  bpl storeVol
  lda #0
storeVol:
  sta MIDFILES.adr.CHNVOLUME,x

  lsr @
  and #%00111100

  add #<UVMETER_ADDR
  sta hlpptr
  // lda #>UVMETER_ADDR
  // sta hlpptr+1

  txa                       //2
  asl @                     //2
  tay                       //2
  lda adr.CHANNELSCRADR,y   //4
  sta scradr                //3
  lda adr.CHANNELSCRADR+1,y //4
  sta scradr+1              //3

  ldy #3          // 2
loop:
  lda hlpptr:$3500,y      // 4*
  sta scradr:$2000,y      // 4*
  // lda (hlpptr),y  // 5*
  // sta (scradr),y  // 6
  dey             // 2
  bpl loop        // 2**

dontUpdate:
  inx
  cpx #16
  bne uvmeterLoop

  ldx regx:#00 // pla:tax