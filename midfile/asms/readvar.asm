A equ _delta;
B equ _delta+1;
C equ _delta+2;
D equ _delta+3;

  .MACRO m@read2A
    ldy #0
    lda (_PTR),y
    sta A
    inc _ADR
    bne skipMemBoundCheck
    inc _ADR+1
    jsr MEMBOUNDCHECK
  skipMemBoundCheck:
    lda A
  .ENDM

  ldy #0
  sty A
  sty B
  sty C
  sty D

// read block

// read 1st byte to A
  m@read2A

  clc
  sne
  rts

  bpl endRead

// move A to B and read 2nd byte to A
// B <- A = 4th readed byte
	and #$7F
	sta B

// read 2nd byte
  m@read2A

  bpl endRead

// mova B to C, A to B and read 3rd byte to A
// C <- B <- A = 4th readed byte
	lda B
	sta C
	lda A
	and #$7F
	sta B

// read 3rd
  m@read2A

  bpl endRead

// move C to D, B to C, A to B and read 4th byte to A
// D <- C <- B <- A = 4th readed byte
	lda C
	sta D
	lda B
	sta C
	lda A
	and #$7F
	sta B

// read 4th
  m@read2A

endRead:
// end read block
// --------------

// ------------
// decode block
// 'A' stays as it is

// 'B' if geather than zero, lets decode
  lda B
  beq decodeC

// 76543210
// 0bbbbbbb and %1
// 0000000b
//        |
// +------+ 7:asl
// v
// b0000000 ora A
// baaaaaaa

  lsr B
  bcc decodeC

  lda A
  ora #$80
  sta A

decodeC:
// 'C' if geather than zero, lets decode
  lda C
  beq decodeD

// 76543210
// 0ccccccc and %11
// 000000cc
//       ||
// +-----+| 6:asl
// |+-----+
// vv
// cc000000 ora B
// ccbbbbbb
  and #%11    // 2
  beq noOrC   // 2**
  tay         // 2
  lda OrC,y   // 4*
  ora B       // 3
  sta B       // 3

noOrC:
  lda C       // 3
  lsr @       // 2
  lsr @       // 2
  sta C       // 3
              //=26*+**

decodeD:
// 'D' if geather than zero, lets decode
  lda D
  beq setC

// 76543210
// 0ddddddd and %111
// 00000ddd
//      |||
// +----+||
// |+----+| 5:asl
// ||+----+
// vvv
// ddd00000 ora C
// dddccccc

  and #%111   // 2
  beq noOrD   // 2**
  tay         // 2
  lda OrD,y   // 4*
  ora C       // 3
  sta C       // 3

noOrD:
  lda D       // 3
  lsr @       // 2
  lsr @       // 2
  lsr @       // 2
  sta D       // 3
              //=28*+**
setC:
  sec

endDecode:
  rts

OrC:
  dta %00000000
  dta %01000000
  dta %10000000
  dta %11000000

OrD:
  dta %00000000
  dta %00100000
  dta %01000000
  dta %01100000
  dta %10000000
  dta %10100000
  dta %11000000
  dta %11100000
