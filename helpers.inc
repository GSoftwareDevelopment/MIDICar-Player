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
  l:=byte(s[0]);
  move(s[1],pointer(scradr),l);
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

procedure putInt(value:smallint); assembler;
asm
EOL = 0

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
    sta _str,X
    inc _cnt
skipDigit:
    iny
    iny
    cpy #8
    bcc ITSNEXT
    lda value
    ora #$10
    ldx _cnt
    sta _str,X
    inx

put2screen:
    lda scradr
    sta screen+1
    lda scradr+1
    sta screen+2

    txa
    tay
putloop:
    dey
    lda _str,y
    ora puttextinvert
screen:
    sta $ffff,y
    cpy #0
    bne putloop

    txa
    clc
    adc scradr
    sta scradr
    bcc stop
    inc scradr+1

stop:
    rts

_cnt .by $00
_str .by 0,0,0,0,0,0
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