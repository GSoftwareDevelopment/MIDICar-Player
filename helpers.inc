const
  GM_RESET: array[0..5] of byte = ($f0, $7e, $7f, $09, $01, $f7);
  hex:array[0..15] of char = (
    '0'~,'1'~,'2'~,'3'~,'4'~,'5'~,'6'~,'7'~,
    '8'~,'9'~,'a'~,'b'~,'c'~,'d'~,'e'~,'f'~
  );

  k_esc = 28;
  k_space = 33;
  k_p = 10;

var
  keyb:byte absolute $2fc;
  puttextinvert:byte;

procedure PutText(s:PString);
var
  l:byte;

begin
  l:=byte(s[0]); inc(s,1);
  move(s,pointer(scradr),l);
  inc(scradr,l);
end;

procedure PutHex(var v; n:byte);
var
  a:byte;
  p:^Byte;

begin
  p:=v;
  inc(scradr,n);
  while n>0 do
  begin
    if n and 1=0 then
      a:=p^
    else
      a:=a shr 4;
    poke(scradr,byte(hex[a and $f]) or puttextinvert); dec(scradr);
    dec(n);
  end;
  inc(scradr,n);
end;

// procedure strVal2Mem(_dest:pointer; value:byte; zeros:byte);
// var
//   ptr,digit,v:byte;
//   dest:array[0..0] of byte;
//   base,step:byte;

//   procedure exdigit();
//   begin
//     if (value>=base) then begin digit:=$15; dec(value,base); end else digit:=$10;
//     v:=4;
//     while value>=step do
//     begin
//       dec(base,step);
//       if (value>=base) then begin digit:=digit+v; dec(value,base); exit; end;
//       dec(v);
//     end;
//   end;

// begin
//   dest:=_dest; ptr:=0;
//   base:=255; step:=100; exdigit();
//   if (zeros>=3) then begin dest[ptr]:=digit or puttextinvert; ptr:=ptr+1; end;

//   base:=50; step:=10; exdigit();
//   if (zeros>=2) then begin dest[ptr]:=digit or puttextinvert; ptr:=ptr+1; end;

//   base:=5; step:=1; exdigit();
//   if (zeros>=1) then dest[ptr]:=digit or puttextinvert;
// end;

// procedure PutValue(value:smallint; zeros:byte);
// begin
// //   setPos(x,y);
//   strVal2Mem(pointer(scradr),value,zeros);
//   inc(scradr,zeros);
// End;

{
  the following procedure is based on code from this page
  https://unfinishedbitness.info/2014/09/26/6502-string-to-integer-and-reverse/
  It is modified to avoid leading zeros.
}
procedure putInt(value:smallint); assembler;
asm
EOL = 0

    lda scradr
    sta screen1
    sta screen2
    lda scradr+1
    sta screen1+1
    sta screen2+1

INT2STR:
    ldy #0
    sty _cnt
ITSNEXT:
    ldx #0
ITSSLP:
    lda value
    sec
    sbc Pow10Tab,Y
    sta value
    lda value+1
    iny
    sbc Pow10Tab,Y
    bcc ITSADD
    sta value+1
    inx
    dey
    clc
    bcc ITSSLP
ITSADD:
    dey
    lda value
    adc Pow10Tab,Y
    sta value
    txa
    ora #$10
    ldx _cnt
    bne storeDigit
    cmp #$10
    beq skipDigit
storeDigit:
    ora puttextinvert
    sta screen1:$f000,X
    inc _cnt
skipDigit:
    iny
    iny
    cpy #8
    bcc ITSNEXT
    lda value
    ora #$10
    ldx _cnt
    ora puttextinvert
    sta screen2:$f000,X
    inx

// set screen addres to the end of value
    txa
    clc
    adc scradr
    sta scradr
    bcc stop
    inc scradr+1

stop:
    rts

_cnt
    .by 0
Pow10Tab
    .wo 10000
    .wo 1000
    .wo 100
    .wo 10
end;

procedure reset_MIDI;
begin
  FIFO_Send(GM_Reset,6); FIFO_Flush;
end;

procedure exit2DOS;
begin
  Cls(0); Opn(0,12,0,'E:');
  halt;
end;