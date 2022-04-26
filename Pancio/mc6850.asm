;  mc6850.asm
;  
;  Copyright 2022 pancio <pancio@desktop>
;  
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;  
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;  
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
;  MA 02110-1301, USA.
;  
;  

; include definitions:
         ICL "mc6850.inc" 


; mc6850_reset:
; MC6850 Master Reset 
mc6850_reset
        lda #(CR0 + CR1)        ;   Master Reset
        sta MC6850_CNTRREG
        rts

; mc6850_init:
; prepare MC6850 for MIDI transmission (31250hz). 
; Base clock 2MHz is divided by 64 
mc6850_init
        lda #(CR1 + CR2 + CR4)  ;   f/64, 8 Bit + 1 stop Bit
        sta MC6850_CNTRREG          
        rts

;mc6850_receive        
; A - returned byte from MC6850_BUFFER
mc6850_receive:
        lda #RDRF
?loop   bit MC6850_CNTRREG
        beq ?loop
        lda MC6850_BUFFER
        rts
        
;mc6850 send
; A -  data to send to MC6850_BUFFER  
mc6850_send:
        pha
        lda #TDRE
?loop   bit MC6850_CNTRREG
        beq ?loop
        pla
        sta MC6850_BUFFER
        rts  


;MIDI_note_on
;A - channel (0-15)
;X - pitch value (note: 0-127)
;Y - volocity value(0-127)   
MIDI_note_on:

    and #%00001111
    ora #%10010000  ;   Status Byte and channel
    jsr mc6850_send
     
    txa
    and #%01111111  ;   pitch (note)
    jsr mc6850_send
    
    tya
    and #%01111111  ;   velocity
    jsr mc6850_send
    
    rts
        
 


