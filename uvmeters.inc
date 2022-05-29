if (playerStatus and ps_view)<>0 then
begin
  chn:=0;
  repeat
    scradr:=channelScrAdr[chn];
    c:=chnVolume[chn];
    if c<>0 then
    begin
      if c>4 then dec(c,4) else c:=0;
      chnVolume[chn]:=c;
      v:=(c shr 1) and %00111100;
      asm
        clc
        lda #<UVMETER_ADDR
        adc v
        sta $d6
        lda #>UVMETER_ADDR
        sta $d7
        ldy #3
      loop:
        lda ($d6),y
        sta ($d4),y
        dey
        bpl loop
      end;
    end;

    inc(chn);
  until (chn=16);
end;