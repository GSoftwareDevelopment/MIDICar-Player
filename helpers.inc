const
  colors:Array[0..1,0..4] of byte = (
    (0,0,10,0,10),
    (10,10,0,0,0)
  );

  GM_RESET: array[0..5] of byte = ($f0, $7e, $7f, $09, $01, $f7);

  k_l     = 0;
  k_plus  = 6;
  k_p     = 10;
  k_minus = 14;
  k_eq    = 15;
  k_esc   = 28;
  k_space = 33;
  k_m     = 37;
  k_invers= 39;
  k_s     = 62;

var
  keyb:byte absolute $2fc;
  puttextinvert:byte;

  NMIEN:Byte absolute $D40E;
  NMIVEC:Pointer absolute $fffa;
  SDMACTL:Byte;
  DMACTL:Byte absolute $d400;
  CHBASE:Byte absolute 756;
  SOUNDR:Byte absolute $41;

procedure setColors;
var p:pointer;

begin
  p:=@colors[byte((playerStatus and ps_colorSet)=ps_colorSet),0];
  move(p,pointer($d016),5);
  move(p,pointer(708),5);
end;

procedure PutText(s:PString);
var
  l,c:byte;

begin
  l:=byte(s[0]);
  while l>0 do
  begin
    inc(s);
    c:=byte(s^);
    if c<32 then inc(c,64)
    else if c<96 then dec(c,32);
    poke(scradr,c or puttextinvert);
    inc(scradr);
    dec(l);
  end;
end;

procedure PutHex(var v; n:byte); Assembler;
asm
  icl 'asms/puthex.a65'
end;

{
  the following procedure is based on code from this page
  https://unfinishedbitness.info/2014/09/26/6502-string-to-integer-and-reverse/
  It is modified to avoid leading zeros.
}
procedure putInt(value:smallint); assembler;
asm
  icl 'asms/int2str.a65'
end;

procedure reset_MIDI;
begin
  FIFO_Send(GM_Reset,6); FIFO_Flush;
end;

procedure clearWorkArea;
begin
  fillchar(pointer(SCREEN_ADDR+80),SCREEN_TIME-SCREEN_ADDR-80,0);
end;

procedure isLooped;
begin
  scradr:=SCREEN_TIME;
  if (playerStatus and ps_loop)<>0 then
    putText('LOOP')
  else
    putText('    ');
end;

procedure clearStatus;
begin
  fillchar(pointer(SCREEN_STATUS-20),60,0);
  isLooped;
  if length(fn)>16 then fn[0]:=#16;
  scradr:=SCREEN_STATUS+40-byte(fn[0]); putText(fn);
end;

procedure clearUVMeters;
begin
  if playerStatus and ps_view<>0 then
  begin
    chn:=0;
    repeat
      scradr:=channelScrAdr[chn];
      fillChar(pointer(scradr),4,$40);
      inc(chn);
    until chn=16;
  end;
end;


procedure exit2DOS;
begin
  Cls(0); Opn(0,12,0,'E:'); SOUNDR:=1;
  halt;
end;