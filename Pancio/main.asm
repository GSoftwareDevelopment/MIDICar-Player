;  main.asm
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

;configuration:
IOPORT_base_address equ $d500

; variable in OS:

keycode equ $02fc
random  equ $d20a

;ROM procedures
putscreen   equ $f1a4


;variables on 0 page        
        org $80
        
zp_channel  .ds 1
zp_pitch    .ds 1
ZP_velocity .ds 1

zp_buffer   .ds 1

a       .ds 1
 


        org   $6000          

;main program         
        
         
start:  

init:
        jsr mc6850_reset
        jsr mc6850_init          



        lda #$00    ;channel0
        ldx #$3c    ;note: middle C
        ldy #$7f    ;max velocity  
        jsr MIDI_note_on
        
        lda #$00
?loop   adc #$01
        bne ?loop
           
        
        lda #$01    ;channel1
        ldx #$40    ;note: middle E
        ldy #$7f    ;max velocity  
        jsr MIDI_note_on

        lda #$00
?loop   adc #$01
        bne ?loop        
        
        lda #$02    ;channel2
        ldx #$43    ;note: middle G
        ldy #$7f    ;max velocity  
        jsr MIDI_note_on
                
        rts              
   

        
        
    ICL "mc6850.asm" 
    
        

