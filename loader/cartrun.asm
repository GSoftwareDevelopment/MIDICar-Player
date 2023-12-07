; ------------------------------------------------------------
; Mad Pascal Compiler version 1.6.9 [2023/08/25] for 6502
; ------------------------------------------------------------

STACKWIDTH	= 16
CODEORIGIN	= $4000

TRUE		= 1
FALSE		= 0

; ------------------------------------------------------------

	org $80

zpage

.if .def(@vbxe_detect)
fxptr	.ds 2						; VBXE pointer
.fi

.if .def(@AllocMem)||.def(MAIN.SYSTEM.GETMEM)||.def(MAIN.SYSTEM.FREEMEM)
psptr	.ds 2						; PROGRAMSTACK Pointer
.fi

bp	.ds 2
bp2	.ds 2

eax	.ds 4						;8 bytes (aex + edx) -> divREAL
edx	.ds 4
ecx	.ds 4

TMP
ztmp
ztmp8	.ds 1
ztmp9	.ds 1
ztmp10	.ds 1
ztmp11	.ds 1

STACKORIGIN	.ds STACKWIDTH*4
zpend

; ------------------------------------------------------------

ax	= eax
al	= eax
ah	= eax+1

cx	= ecx
cl	= ecx
ch	= ecx+1

dx	= edx
dl	= edx
dh	= edx+1

	org eax

FP1MAN0	.ds 1
FP1MAN1	.ds 1
FP1MAN2	.ds 1
FP1MAN3	.ds 1

	org ztmp8

FP1SGN	.ds 1
FP1EXP	.ds 1

	org edx

FP2MAN0	.ds 1
FP2MAN1	.ds 1
FP2MAN2	.ds 1
FP2MAN3	.ds 1

	org ztmp10

FP2SGN	.ds 1
FP2EXP	.ds 1

	org ecx

FPMAN0	.ds 1
FPMAN1	.ds 1
FPMAN2	.ds 1
FPMAN3	.ds 1

	org bp2

FPSGN	.ds 1
FPEXP	.ds 1

	.ifdef MAIN.@DEFINES.BASICOFF
	org CODEORIGIN
	icl 'atari\basicoff.asm'
	ini CODEORIGIN
	.fi

	.ifdef MAIN.@DEFINES.S_VBXE
	opt h-
	ins 'atari\s_vbxe\sdxld2.obx'
	opt h+
	.fi

	org CODEORIGIN

.local	MAIN.@RESOURCE
.endl

.local	RESOURCE
	icl 'atari\resource.asm'
	?EXTDETECT = 0
	?VBXDETECT = 0

.endl

; ------------------------------------------------------------

	org CODEORIGIN

	STATICDATA

; ------------------------------------------------------------

RTLIB
	icl 'rtl6502_a8.asm'

.print 'ZPAGE: ',zpage,'..',zpend-1

.print 'RTBUF: ',@buf,'..',@buf+255

.print 'RTLIB: ',RTLIB,'..',*-1

; ------------------------------------------------------------

START
	tsx
	stx MAIN.@halt+1

VLEN	= VARDATASIZE-VARINITSIZE
VADR	= DATAORIGIN+VARINITSIZE

	ift VADR > $BFFF
	ert 'Invalid memory address range ',VADR
	eli (VLEN > 0) && (VLEN <= 256)
	ldx #256-VLEN
	lda #$00
	sta:rne VADR+VLEN-256,x+
	eli VLEN>256
	m@init
	eif

.ifdef psptr
	mwa #PROGRAMSTACK psptr
.fi

	.ifdef MAIN.@DEFINES.ROMOFF
	icl 'atari\romoff.asm'
	.fi

	ldx #$0F					; DOS II+/D ParamStr
	mva:rpl $340,x MAIN.IOCB@COPY,x-

	inx						; X = 0
	stx bp						; BP = 0

	stx audctl					; reset POKEY
	stx audctl+$10
	lda #3
	sta skctl
	sta skctl+$10

	dex						; X = 255

	UNITINITIALIZATION

.local	MAIN						; PROCEDURE

	jmp l_002B

; ------------------------------------------------------------

.local	SYSTEM						; UNIT

; ------------------------------------------------------------

__PORTB_BANKS	= $0101
M_PI_2	= $0648
D_PI_2	= $0192
D_PI_180	= $04
MGTIA	= $00
MVBXE	= $80
VBXE_XDLADR	= $00
VBXE_BCBTMP	= $E0
VBXE_BCBADR	= $0100
VBXE_MAPADR	= $1000
VBXE_CHBASE	= $1000
VBXE_OVRADR	= $5000
VBXE_WINDOW	= $B000
IDLI	= $00
IVBL	= $01
IVBLD	= $01
IVBLI	= $02
ITIM1	= $03
ITIM2	= $04
ITIM4	= $05
CH_DELCHR	= $FE
CH_ENTER	= $9B
CH_ESC	= $1B
CH_CURS_UP	= $1C
CH_CURS_DOWN	= $1D
CH_CURS_LEFT	= $1E
CH_CURS_RIGHT	= $1F
CH_TAB	= $7F
CH_EOL	= $9B
CH_CLR	= $7D
CH_BELL	= $FD
CH_DEL	= $7E
CH_DELLINE	= $9C
CH_INSLINE	= $9D
PAL_PMCOLOR0	= $00
PAL_PMCOLOR1	= $01
PAL_PMCOLOR2	= $02
PAL_PMCOLOR3	= $03
PAL_COLOR0	= $04
PAL_COLOR1	= $05
PAL_COLOR2	= $06
PAL_COLOR3	= $07
PAL_COLBAK	= $08
COLOR_BLACK	= $00
COLOR_WHITE	= $0E
COLOR_RED	= $32
COLOR_CYAN	= $96
COLOR_VIOLET	= $68
COLOR_GREEN	= $C4
COLOR_BLUE	= $74
COLOR_YELLOW	= $EE
COLOR_ORANGE	= $28
COLOR_BROWN	= $E4
COLOR_LIGHTRED	= $3C
COLOR_GRAY1	= $04
COLOR_GRAY2	= $06
COLOR_GRAY3	= $0A
COLOR_LIGHTGREEN	= $CC
COLOR_LIGHTBLUE	= $7C
FMOPENREAD	= $04
FMOPENWRITE	= $08
FMOPENAPPEND	= $09
FMOPENREADWRITE	= $0C
SCREENWIDTH	= DATAORIGIN+$0000
SCREENHEIGHT	= DATAORIGIN+$0002
DATESEPARATOR	= DATAORIGIN+$0004
RND	= $D20A
adr.PALETTE	= $02C0
.var PALETTE	= adr.PALETTE .word
adr.HPALETTE	= $D012
.var HPALETTE	= adr.HPALETTE .word
FILEMODE	= DATAORIGIN+$0005
GRAPHMODE	= DATAORIGIN+$0006
IORESULT	= DATAORIGIN+$0007
EOLN	= DATAORIGIN+$0008
RNDSEED	= DATAORIGIN+$0009

.endl							; UNIT SYSTEM

; ------------------------------------------------------------

.local	ATARI						; UNIT

; ------------------------------------------------------------

IRQENS	= $10
RTCLOK	= $12
RTCLOK1	= $12
RTCLOK2	= $13
RTCLOK3	= $14
ATRACT	= $4D
LMARGIN	= $52
RMARGIN	= $53
ROWCRS	= $54
COLCRS	= $55
DINDEX	= $57
SAVMSC	= $58
PALNTS	= $62
RAMTOP	= $6A
VDSLST	= $0200
SDLSTL	= $0230
TXTROW	= $0290
TXTCOL	= $0291
TINDEX	= $0293
TXTMSC	= $0294
SDMCTL	= $022F
GPRIOR	= $026F
CRSINH	= $02F0
CHACT	= $02F3
CHBAS	= $02F4
CH	= $02FC
FILDAT	= $02FD
PCOLR0	= $02C0
PCOLR1	= $02C1
PCOLR2	= $02C2
PCOLR3	= $02C3
COLOR0	= $02C4
COLOR1	= $02C5
COLOR2	= $02C6
COLOR3	= $02C7
COLOR4	= $02C8
COLBAKS	= $02C8
HPOSP0	= $D000
HPOSP1	= $D001
HPOSP2	= $D002
HPOSP3	= $D003
HPOSM0	= $D004
HPOSM1	= $D005
HPOSM2	= $D006
HPOSM3	= $D007
SIZEP0	= $D008
SIZEP1	= $D009
SIZEP2	= $D00A
SIZEP3	= $D00B
SIZEM	= $D00C
GRAFP0	= $D00D
GRAFP1	= $D00E
GRAFP2	= $D00F
GRAFP3	= $D010
GRAFM	= $D011
P0PF	= $D004
PAL	= $D014
TRIG3	= $D013
COLPM0	= $D012
COLPM1	= $D013
COLPM2	= $D014
COLPM3	= $D015
COLPF0	= $D016
COLPF1	= $D017
COLPF2	= $D018
COLPF3	= $D019
COLBK	= $D01A
PRIOR	= $D01B
GRACTL	= $D01D
HITCLR	= $D01E
CONSOL	= $D01F
AUDF1	= $D200
AUDC1	= $D201
AUDF2	= $D202
AUDC2	= $D203
AUDF3	= $D204
AUDC3	= $D205
AUDF4	= $D206
AUDC4	= $D207
AUDCTL	= $D208
KBCODE	= $D209
IRQEN	= $D20E
SKSTAT	= $D20F
PORTA	= $D300
PORTB	= $D301
PACTL	= $D302
DMACTL	= $D400
CHACTL	= $D401
DLISTL	= $D402
HSCROL	= $D404
VSCROL	= $D405
PMBASE	= $D407
CHBASE	= $D409
WSYNC	= $D40A
VCOUNT	= $D40B
PENH	= $D40C
PENV	= $D40D
NMIEN	= $D40E
NMIVEC	= $FFFA
RESETVEC	= $FFFC
IRQVEC	= $FFFE

.endl							; UNIT ATARI

; ------------------------------------------------------------

.local	ZX5						; UNIT

.local	UNZX5_00A5					; PROCEDURE | ASSEMBLER | OVERLOAD

; -------------------  ASM Block 00000061  -------------------

ZX5_OUTPUT      equ :EAX+0
copysrc         equ :EAX+2
offset          equ :EAX+4
offset2         equ :EAX+6
offset3         equ :EAX+8
len             equ :EAX+$A
pnb             equ :EAX+$C

unZX5		stx @sp

		mwa inputPointer ZX5_INPUT
		mwa outputPointer ZX5_OUTPUT

		lda   #$ff
		sta   offset
		sta   offset+1
		ldy   #$00
		sty   len
		sty   len+1
		lda   #$80

dzx5s_literals
		jsr   dzx5s_elias
		pha
cop0		jsr   _GET_BYTE
		ldy   #$00
		sta   (ZX5_OUTPUT),y
		inw   ZX5_OUTPUT
		lda   len
		bne   @+
		dec   len+1
@		dec   len
		bne   cop0
		lda   len+1
		bne   cop0
		pla
		asl   @
		bcs   dzx5s_other_offset

dzx5s_last_offset
		jsr   dzx5s_elias
dzx5s_copy	pha
		lda   ZX5_OUTPUT
		clc
		adc   offset
		sta   copysrc
		lda   ZX5_OUTPUT+1
		adc   offset+1
		sta   copysrc+1
		ldy   #$00
		ldx   len+1
		beq   Remainder
Page		lda   (copysrc),y
		sta   (ZX5_OUTPUT),y
		iny
		bne   Page
		inc   copysrc+1
		inc   ZX5_OUTPUT+1
		dex
		bne   Page
Remainder	ldx   len
		beq   copyDone
copyByte	lda   (copysrc),y
		sta   (ZX5_OUTPUT),y
		iny
		dex
		bne   copyByte
		tya
		clc
		adc   ZX5_OUTPUT
		sta   ZX5_OUTPUT
		bcc   copyDone
		inc   ZX5_OUTPUT+1
copyDone	stx   len+1
		stx   len
		pla
		asl   @
		bcc   dzx5s_literals

dzx5s_other_offset
		asl   @
		bne   dzx5s_other_offset_skip
		jsr   _GET_BYTE
		sec	; można usunąć jeśli dekompresja z pamięci a nie pliku
		rol   @
dzx5s_other_offset_skip
		bcc   dzx5s_prev_offset

dzx5s_new_offset
		sta   pnb
		asl   @
		ldx   offset2
		stx   offset3
		ldx   offset2+1
		stx   offset3+1
		ldx   offset
		stx   offset2
		ldx   offset+1
		stx   offset2+1
		ldx   #$fe
		stx   len
		jsr   dzx5s_elias_loop
		pha
		ldx   len
		inx
		stx   offset+1
		bne   @+
		pla

		jmp to_exit	; koniec

@		jsr   _GET_BYTE
		sta   offset
		ldx   #$00
		stx   len+1
		inx
		stx   len
		pla
		dec   pnb
		bmi   @+
		jsr   dzx5s_elias_backtrack
@		inw   len
		jmp   dzx5s_copy

dzx5s_prev_offset
		asl   @
		bcc   dzx5s_second_offset
		ldy   offset2
		ldx   offset3
		sty   offset3
		stx   offset2
		ldy   offset2+1
		ldx   offset3+1
		sty   offset3+1
		stx   offset2+1

dzx5s_second_offset
		ldy   offset2
		ldx   offset
		sty   offset
		stx   offset2
		ldy   offset2+1
		ldx   offset+1
		sty   offset+1
		stx   offset2+1
		jmp   dzx5s_last_offset

dzx5s_elias	inc   len
dzx5s_elias_loop
		asl   @
		bne   dzx5s_elias_skip
		jsr   _GET_BYTE
		sec	; można usunąć jeśli dekompresja z pamięci a nie pliku
		rol   @
dzx5s_elias_skip
		bcc   dzx5s_elias_backtrack
		rts

dzx5s_elias_backtrack
		asl   @
		rol   len
		rol   len+1
		jmp   dzx5s_elias_loop

_GET_BYTE	lda    $ffff
ZX5_INPUT	equ    *-2
		inw    ZX5_INPUT
		rts

to_exit		ldx #0
@sp		equ *-1

; ------------------------------------------------------------

INPUTPOINTER	= DATAORIGIN+$000B
OUTPUTPOINTER	= DATAORIGIN+$000D

@VarData	= INPUTPOINTER
@VarDataSize	= 4

@exit
	.ifdef @new
	lda <@VarData
	sta :ztmp
	lda >@VarData
	ldy #@VarDataSize-1
	jmp @FreeMem
	els
	rts						; ret
	eif
.endl

; ------------------------------------------------------------

.endl							; UNIT ZX5
l_002B

; optimize FAIL ('@print', cartrun.pas), line = 9

	@printSTRING #CODEORIGIN+$0000
	@printEOL

; GetResourceHandle
	lda <MAIN.@RESOURCE.VARS
	sta DPTR
	lda >MAIN.@RESOURCE.VARS
	sta DPTR+1

; optimize OK (cartrun.pas), line = 11

	lda DPTR
	sta ZX5.UNZX5_00A5.INPUTPOINTER
	lda DPTR+1
	sta ZX5.UNZX5_00A5.INPUTPOINTER+1
	lda #$00
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER
	lda #$04
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER+1
	jsr ZX5.UNZX5_00A5

; GetResourceHandle
	lda <MAIN.@RESOURCE.PROG
	sta DPTR
	lda >MAIN.@RESOURCE.PROG
	sta DPTR+1

; optimize OK (cartrun.pas), line = 13

	lda DPTR
	sta ZX5.UNZX5_00A5.INPUTPOINTER
	lda DPTR+1
	sta ZX5.UNZX5_00A5.INPUTPOINTER+1
	lda #$00
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER
	lda #$80
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER+1
	jsr ZX5.UNZX5_00A5

; GetResourceHandle
	lda <MAIN.@RESOURCE.DATA
	sta DPTR
	lda >MAIN.@RESOURCE.DATA
	sta DPTR+1

; optimize OK (cartrun.pas), line = 15

	lda DPTR
	sta ZX5.UNZX5_00A5.INPUTPOINTER
	lda DPTR+1
	sta ZX5.UNZX5_00A5.INPUTPOINTER+1
	lda #$80
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER
	lda #$22
	sta ZX5.UNZX5_00A5.OUTPUTPOINTER+1
	jsr ZX5.UNZX5_00A5

; -------------------  ASM Block 00000062  -------------------

    jmp $8857
  
; ------------------------------------------------------------

DPTR	= DATAORIGIN+$000F
@exit

@halt	ldx #$00
	txs
	.ifdef MAIN.@DEFINES.ROMOFF
	inc portb
	.fi

	ldy #$01

	rts

; ------------------------------------------------------------

IOCB@COPY	:16 brk

; ------------------------------------------------------------

.local	@DEFINES
ATARI
ROMOFF
.endl

.local	@RESOURCE
VARS	ins 'mcp-vars.zx5'
VARS.end
DATA	ins 'mcp-data.zx5'
DATA.end
PROG	ins 'mcp-prog.zx5'
PROG.end
.endl

.endl							; MAIN

; ------------------------------------------------------------
; ------------------------------------------------------------

.macro	UNITINITIALIZATION

	.ifdef MAIN.SYSTEM.@UnitInit
	jsr MAIN.SYSTEM.@UnitInit
	.fi

	.ifdef MAIN.ATARI.@UnitInit
	jsr MAIN.ATARI.@UnitInit
	.fi

	.ifdef MAIN.ZX5.@UnitInit
	jsr MAIN.ZX5.@UnitInit
	.fi
.endm

; ------------------------------------------------------------

	ift .SIZEOF(MAIN.SYSTEM) > 0
	.print 'SYSTEM: ',MAIN.SYSTEM,'..',MAIN.SYSTEM+.SIZEOF(MAIN.SYSTEM)-1
	eif

	ift .SIZEOF(MAIN.ATARI) > 0
	.print 'ATARI: ',MAIN.ATARI,'..',MAIN.ATARI+.SIZEOF(MAIN.ATARI)-1
	eif

	ift .SIZEOF(MAIN.ZX5) > 0
	.print 'ZX5: ',MAIN.ZX5,'..',MAIN.ZX5+.SIZEOF(MAIN.ZX5)-1
	eif

.nowarn	.print 'CODE: ',CODEORIGIN,'..',MAIN.@RESOURCE-1
	.print '$R VARS',' ',"'mcp-vars.zx5'",' ',MAIN.@RESOURCE.VARS,'..',MAIN.@RESOURCE.VARS.end-1
	.print '$R DATA',' ',"'mcp-data.zx5'",' ',MAIN.@RESOURCE.DATA,'..',MAIN.@RESOURCE.DATA.end-1
	.print '$R PROG',' ',"'mcp-prog.zx5'",' ',MAIN.@RESOURCE.PROG,'..',MAIN.@RESOURCE.PROG.end-1

; ------------------------------------------------------------

	?adr = *
	ift (?adr < ?old_adr) && (?old_adr - ?adr < $120)
	?adr = ?old_adr
	eif

	org ?adr
	?old_adr = *

DATAORIGIN
.by  $28 $00 $18 $00 $2D $0C

VARINITSIZE	= *-DATAORIGIN
VARDATASIZE	= 17

PROGRAMSTACK	= DATAORIGIN+VARDATASIZE

	.print 'DATA: ',DATAORIGIN,'..',PROGRAMSTACK

	run START

; ------------------------------------------------------------

.macro	STATICDATA
.by  $10 $64 $65 $63 $6F $6D $70 $72  $65 $73 $73 $69 $6E $67 $2E $2E  $2E $00 $04 $56 $41 $52 $53 $00
.by  $04 $50 $52 $4F $47 $00 $04 $44  $41 $54 $41 $00
.endm

	end
