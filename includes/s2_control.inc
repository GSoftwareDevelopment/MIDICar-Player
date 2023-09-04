const
  ControlMsgs : array[0..4] of String =
  (
    #$4e+'VOLUME'*~+#$ce,
    #$4e+'EQ'~*+#$ce+#$4e+'LOW'~*+#$ce+'MID HIGH '~,
    #$4e+'EQ'~*+#$ce+#$4e+'LOW MID'~*+#$ce+'HIGH '~,
    #$4e+'EQ'~*+#$ce+' LOW'~+#$4e+'MID HIGH'~*+#$ce,
    #$4e+'EQ'~*+#$ce+' LOW MID'~+#$4e+'HIGH'~*+#$ce
  );

  ctrlReg:Array[0..4] of byte = (
    NRPN_MasterVolume,
    NRPN_EQ_Low,
    NRPN_EQ_MLow,
    NRPN_EQ_MHigh,
    NRPN_EQ_High
  );

var
  curControl:Byte;
  oldControl:Byte = 255;

procedure drawScale; Assembler; Keep;
asm
  icl 'asms/osd_scale_draw.a65'
end;

procedure changeOSDMsg; Keep;
begin
  scradr:=SCREEN_OSD; HLine;
  scradr:=SCREEN_OSD+1; PutINTText(ControlMsgs[curControl]);
end;

procedure ControlSetting; Keep;
begin
  asm
    lda screenStatus
    and #ss_isOSD
    bne skipControlReset
    lda #0
    sta curControl
  skipControlReset:

    lda MAIN.KEYS.keyb    // check pressed key "E"
    cmp #MAIN.KEYS.k_E
    bne default_volume_control

    ldy curControl
    iny
    cpy #5
    bne eq_control
    ldy #0

  eq_control:
    sty curControl
    lda ADR.ctrlReg,y
    tay
    lda MAIN.NRPM.ADR.NRPN_Regs,y
    sta v
    jsr MAIN.drawScale     // draw scale

  default_volume_control:
    lda curControl
    cmp oldControl
    beq dontChangeMsg
    sta oldControl
    jsr MAIN.changeOSDMsg
  dontChangeMsg:

    jsr MAIN.OSDOn

    ldy curControl
    lda ADR.ctrlReg,y
    tay

    lda MAIN.KEYS.keyb      // check pressed key "."
    cmp #MAIN.KEYS.k_DOT
    bne nextKey

    lda MAIN.NRPM.ADR.NRPN_Regs,y     // get value from NRPN Register
    add #2                  // increase value
    bpl sendEvent
    lda #127
    bpl sendEvent           // always jump

  nextKey:
    cmp #MAIN.KEYS.k_COMMA  // check pressed key ","
    bne endVolumeChange

    lda MAIN.NRPM.ADR.NRPN_Regs,y     // get value from NRPN Register
    sub #2                  // decrease value
    bpl sendEvent
    lda #0

  sendEvent:
    sta MAIN.NRPM.ADR.NRPN_Regs,y     // store changed value in NRPN Register
    sta v                   // `v` catch value for draw scale procedure
    jsr MAIN.NRPM.DBS2_sendControlEvent.directSet  // send NRPN Event
    jsr MAIN.drawScale                        // draw scale

  endVolumeChange:
  end;
end;