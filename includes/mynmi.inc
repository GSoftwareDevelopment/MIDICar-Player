const
  thumblr:array[0..3] of byte = ($a9,$9c,$9d,$9e);

var
  VVBLKI:Pointer absolute $222;
  NMIEN:Byte absolute $D40E;
  NMIVEC:Pointer absolute $FFFA;
  oldVVBLKI,
  oldNMIVec:Pointer;
  thi:Byte absolute $1f;

procedure MyNMI; Interrupt; Assembler;
asm
  icl 'asms/nmi.a65'
end;

procedure ImmediateVBLK; Interrupt; Assembler;
asm
  icl 'asms/immediateVB.a65'
end;

procedure setNMI;
begin
  oldNMIVec:=NMIVEC;
  oldVVBLKI:=VVBLKI;
  NMIEN:=$00;
  asm
    txa:pha
    lda VVBLKI
    sta ImmediateVBLK.OLD_VBLKI
    lda VVBLKI+1
    sta ImmediateVBLK.OLD_VBLKI+1
    inc PORTB
    lda #6
    ldy #<ImmediateVBLK
    ldx #>ImmediateVBLK
    jsr $e45c
    dec PORTB
    pla:tax
  end;
  NMIVEC:=@MyNMI;
  NMIEN:=$40;
end;

procedure unsetNMI;
begin
  NMIEN:=$00;
  VVBLKI:=oldVVBLKI;
  NMIVEC:=oldNMIVec;
  NMIEN:=$40;
end;