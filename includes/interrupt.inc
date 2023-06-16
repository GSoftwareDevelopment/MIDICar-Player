const
  thumblr:array[0..3] of byte = ($a9,$9c,$9d,$9e);
  Yscans:Array[0..10] of byte = (24,29,33,38,42,47,51,56,60,65,69);

var
  DLIV:Pointer absolute $200;
  VVBLKI:Pointer absolute $222;
  NMIEN:Byte absolute $D40E;
  NMIVEC:Pointer absolute $FFFA;
  oldDLIV,
  oldVVBLKI,
  oldNMIVec:Pointer;
  thi:Byte absolute $1f;

{$IFDEF IRQPATCH}
  VIMIRQ:Pointer absolute $216;
  oldVIMIRQ:Pointer;

procedure MyIRQ; Interrupt; Assembler;
asm
  icl 'asms/interrupts/IRQ-Patch.a65'
end;
{$ENDIF}

procedure MyDLI; Interrupt; Assembler;
asm
  icl 'asms/interrupts/DLI.a65'
end;

procedure MyNMI; Interrupt; Assembler;
asm
  icl 'asms/interrupts/NMI.a65'
end;

procedure ImmediateVBLK; Interrupt; Assembler;
asm
  icl 'asms/interrupts/immediateVB.a65'
end;

procedure setNMI;
begin
  oldNMIVec:=NMIVEC;
  oldVVBLKI:=VVBLKI;
  oldDLIV:=DLIV;
  NMIEN:=$00;
  asm
  JSETVBV = $E45C

    txa:pha

  .ifdef MAIN.@DEFINES.IRQPATCH
    lda VIMIRQ
    sta oldVIMIRQ
    sta MyIRQ.OLD_VIMIRQ
    lda VIMIRQ+1
    sta oldVIMIRQ+1
    sta MyIRQ.OLD_VIMIRQ+1
  .endif

    lda VVBLKI
    sta ImmediateVBLK.OLD_VBLKI
    lda VVBLKI+1
    sta ImmediateVBLK.OLD_VBLKI+1

    inc PORTB

    lda #6
    ldy #<ImmediateVBLK
    ldx #>ImmediateVBLK
    jsr JSETVBV

  .ifdef MAIN.@DEFINES.IRQPATCH
    lda #0
    ldy #<MyIRQ
    ldx #>MyIRQ
    jsr JSETVBV
  .endif

    dec PORTB
    pla:tax
  end;
  NMIVEC:=@MyNMI;
  DLIV:=@myDLI;
  NMIEN:=$C0;
end;

procedure unsetNMI;
begin
  NMIEN:=$00;
{$IFDEF IRQPATCH}
  asm
  JSETVBV = $E45C
    inc PORTB
    lda #0
    ldy oldVIMIRQ
    ldx oldVIMIRQ+1
    jsr JSETVBV
    dec PORTB
  end;
{$ENDIF}
  VVBLKI:=oldVVBLKI;
  NMIVEC:=oldNMIVec;
  DLIV:=oldDLIV;
  NMIEN:=$40;
end;