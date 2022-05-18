{

}

procedure int_timer; Interrupt; Assembler;
asm
    lda _timerStatus
    bpl doSubTick
    pla
    rti

doSubTick:
    and #f_ratio
    cmp _subCnt
    beq doMainTick

    inc _subCnt
    pla
    rti

doMainTick:
    lda #1
    sta _subCnt

    inc _totalTicks
    bne skip
    inc _totalTicks+1
    bne skip
    inc _totalTicks+2
    bne skip
    inc _totalTicks+3
    bne skip

skip:

  pla
end;
