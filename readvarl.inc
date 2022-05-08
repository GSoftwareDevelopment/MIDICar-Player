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
    lda (_PTR),y
    sta A
    inc _ADR
    sne
    inc _ADR+1
    jsr MEMBOUNDCHECK
    lda A
  .ENDM

  ldy #$00
  sty A
  sty B
  sty C
  sty D

// read block

// read 1st byte
	// .local +MAIN.MIDFILES.PROCESSTRACK.READB
	// m@INLINE
	// .endl
	// lda _TMP
	// sta A
  m@read2A

  bpl endRead

// move A to B and read 2nd byte
	and #$7F
	sta B

// read 2nd byte
	// .local +MAIN.MIDFILES.PROCESSTRACK.READB
	// m@INLINE
	// .endl
	// lda _TMP
	// sta A

  m@read2A

  bpl endRead

// mova B to C, A to B and read 3rd byte
	lda B
	sta C
	lda A
	and #$7F
	sta B

// read 3rd
	// .local +MAIN.MIDFILES.PROCESSTRACK.READB
	// m@INLINE
	// .endl
	// lda _TMP
	// sta A

  m@read2A

  bpl endRead

// move C to D, B to C, A to B and read 4th byte
	lda C
	sta D
	lda B
	sta C
	lda A
	and #$7F
	sta B

// read 4th
	// .local +MAIN.MIDFILES.PROCESSTRACK.READB
	// m@INLINE
	// .endl
	// lda _TMP
	// sta A

  m@read2A

// end read block
endRead:

// decode block
// 'A' stays as it is

// 'B' if geather than zero, lets decode
  lda B
  beq noDecodeB

// 76543210 76543210
// 0bbbbbbb 0aaaaaaa
// 00bbbbbb Baaaaaaa
  lsr B
  bcc noDecodeB

  lda A
  ora #$80
  sta A

noDecodeB:
// 'C' if geather than zero, lets decode
  lda C
  beq noDecodeC

// 76543210 76543210
// 0ccccccc 0bbbbbbb
// 00cccccc 0Cbbbbbb
  lsr C
  bcc noC1

  lda B
  ora #$40
  sta B

  lda C // reload C

noC1:
// 76543210 76543210
// 0ccccccc 0bbbbbbb
// 000ccccc Ccbbbbbb
  lsr C
  bcc noDecodeC

  lda B
  ora #$80
  sta B

noDecodeC:
// 'D' if geather than zero, lets decode
  lda D
  beq noDecodeD

// 76543210 76543210
// 0ddddddd 0ccccccc
// 00dddddd 00Dccccc
  lsr D
  bcc noD1

  lda C
  ora #$20
  sta C

  lda D // reloaad D
noD1:
// 76543210 76543210
// 0ddddddd 0ccccccc
// 000ddddd 0Ddccccc
  lsr D
  bcc noD2

  lda C
  ora #$40
  sta C

  lda D // reaload D
noD2:
// 76543210 76543210
// 0ddddddd 0ccccccc
// 0000dddd Dddccccc
  lsr D
  bcc noDecodeD

  lda C
  ora #$80
  sta C

noDecodeD:

end;

{
Pascal version of above procedure

var
  a,b,c,d:byte;
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
    if c and 2=2 then b:=b or 128;
    if c and 1=1 then b:=b or 64;
    c:=c shr 2;
  end;
  if d>0 then
  begin
    if d and 4=4 then c:=c or 128;
    if d and 2=2 then c:=c or 64;
    if d and 1=1 then c:=c or 32;
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
