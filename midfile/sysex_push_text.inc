procedure sendClearPushLCD; Keep;
const
  push_sysex:array[0..5] of byte = (
    $f0, $47, $7f, $15, $00, $F7
  );

begin
  asm
  event = $ff

    sei
    jsr $2003
    cli

    ldx #0

  sysex_header_loop:
    lda ADR.PUSH_SYSEX,x
    sta event
    jsr $2006
    inx
    cpx #6
    bne sysex_header_loop

    sei
    sec       ; flush buffer & uninitialize driver
    jsr $200c
    cli
  end;
end;

procedure sendPushLCDText(str:PString); Register; Keep;
const
  push_sysex:array[0..7] of byte = (
    $f0, $47, $7f, $15, $01, $00, $45, $00
  );

begin
  asm
  event = $ff

    lda str
    sta strSrc1
    sta strSrc2
    lda str+1
    sta strSrc1+1
    sta strSrc2+1

    sei
    jsr $2003
    cli

    ldx #0
  sysex_header_loop:
    lda ADR.PUSH_SYSEX,x
    sta event
    jsr $2006
    inx
    cpx #8
    bne sysex_header_loop

  // sysex data loop
    ldx #1
  readLoop:
    lda strSrc2:$ffff,x
    jsr $2006

    inx
    cpx strSrc1:$ffff
    bmi readLoop
    beq readLoop

  // sysex end
    lda #$F7
    sta event
    jsr $2006

    sei
    sec       ; flush buffer & uninitialize driver
    jsr $200c
    cli
  end;
end;