; ; Detect MIDICar
; step1:
;     jsr detect_MC
;     bcs MCNotFound

; MCFound:
;     sty MCBASE
;     stx MCBASE+1

;     jsr MCBASEPORT

;     lda #<MIDICAR_EXIST
;     ldy #>MIDICAR_EXIST
;     jsr PRINT

;     jmp step2

; MCNotFound:
;     lda #$FF
;     sta MCBASE
;     lda #$00
;     sta MCBASE+1
;     lda #$02
;     sta $4ff
;     lda #<MIDICAR_NOT_FOUND
;     ldy #>MIDICAR_NOT_FOUND
;     jsr PRINT