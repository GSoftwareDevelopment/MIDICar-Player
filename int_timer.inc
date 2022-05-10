{

}
procedure int_timer; Interrupt; Assembler;
asm
    lda _timerStatus
    bmi skip

    and #f_ratio
    cmp _subCnt
    bne incSubCounter

    // lda _timerStatus
    // ora #f_tick
    // sta _timerStatus

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

incSubCounter:
    inc _subCnt

skip:

  pla
end;
