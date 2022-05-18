{
A procedure that reads a variable length value called a delta.

The primary task of a delta is to determine the relative time (in ticks)
between MIDI events. It is also used to determine the amount of data for
SysEx events.

The value occurs ONLY in the MID file - it is not part of the MIDI protocol.

---
It is *important* that it is as __fast as possible__.
}

procedure readVarL; Assembler;
asm
A equ _delta;
B equ _delta+1;
C equ _delta+2;
D equ _delta+3;

  .MACRO m@read2A
    ldy #0
    lda (_PTR),y
    sta A
    inc _ADR
    sne
    inc _ADR+1
    jsr MEMBOUNDCHECK
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

// end read block
endRead:

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
  beq endDecode

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
end;

{
Pascal version of above procedure

const
  OrC:array[0..3] of byte = (
    %00000000, %01000000, %10000000, %11000000
  );

  OrD:array[0..7] of byte = (
    %00000000, %00100000, %01000000, %01100000,
    %10000000, %10100000, %11000000, %11100000
  );

var
  a,b,c,d,y:byte;
  resultPTR:^Byte;

begin
  a:=0; b:=0; c:=0; d:=0;

// read block
  readB; a:=_tmp;
  if a and $80=$80 then
  begin
    b:=a and $7f;
    readB; a:=_tmp;
    if a and $80=$80 then
    begin
      c:=b; b:=a and $7f;
      readB; a:=_tmp;
      if a and $80=$80 then
      begin
        d:=c; c:=b; b:=a and $7f;
        readB; a:=_tmp;
      end;
    end;
  end;

// decode block
  if b>0 then
  begin
    if b and 1=1 then a:=a or 128;
    b:=b shr 1;
  end;
  if c>0 then
  begin
    y:=c and %11;
    if y<>0 then
      b:=b or OrC[y];
    c:=c shr 2;
  end;
  if d>0 then
  begin
    y:=d and %111;
    if y<>0 then
      c:=c or OrD[y];
    d:=d shr 3;
  end;

// result block
  ResultPTR:=@Result;
  ResultPTR^:=a; inc(ResultPTR);
  ResultPTR^:=b; inc(ResultPTR);
  ResultPTR^:=c; inc(ResultPTR);
  ResultPTR^:=d;
end;
}

{
  The 7th bit of each byte indicates whether the next delta byte occurs

  One byte of delta
  7654 3210
  0aaa aaaa
 =0aaa aaaa

  Two bytes of delta
  7654 3210 7654 3210
  1bbb bbbb 0aaa aaaa
 =00bb bbbb Baaa aaaa

  Three bytes of delta
  7654 3210 7654 3210 7654 3210
  1ccc cccc 1bbb bbbb 0aaa aaaa
 =000c cccc CCbb bbbb Baaa aaaa

  Four bytes of delta
  7654 3210 7654 3210 7654 3210 7654 3210
  1ddd dddd 1ccc cccc 1bbb bbbb 0aaa aaaa
 =0000 dddd DDDc cccc CCbb bbbb Baaa aaaa
}
