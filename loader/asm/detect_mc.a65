
MC_SETUP = $16
MC_RESET = $03

; result:
; C - MC found
; A,Y - base of MC

detect_MC:
    ldx #$d5

MCBegin:
    ldy #$00

detectLoop:
    stx *+5
    lda $d500,y
    beq test1
    cmp #$02
    bne next

test1:
    sta repval
    stx *+7

loop:
    iny
    iny
    lda $d500,y
    cmp repval:#00
    bne next
    tya
    and #$1f
    cmp #$1e
    bne loop

    tya
    and #%11100000
    tay

test2:
    lda #MC_SETUP
    stx *+5
    sta $d500,y

    stx *+5
    lda $d500,y
    cmp #$02
    beq found

next:
    tya
    and #%11100000
    clc
    adc #$20
    bcs changeMSBBase
    tay
    jmp detectLoop

changeMSBBase:
    cpx #$d1
    beq notFound
    ldx #$d1
    jmp MCBegin

; ------------------

found:
    lda #MC_RESET
    stx *+5
    sta $d500,y
    clc
    rts

notfound:
    ldx #0
    ldy #0
    sec
    rts