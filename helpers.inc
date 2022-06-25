const
  colors:Array[0..1,0..4] of byte = (
    ($A8,$00,$0A,$08,$0A),
    ($A8,$0A,$00,$02,$00)
  );

  GM_RESET: array[0..5] of byte = ($f0, $7e, $7f, $09, $01, $f7);

{$i keys.inc}

var
  keyb:byte absolute $2fc;
  puttextinvert:byte;

  SDMACTL:Byte;
  DMACTL:Byte absolute $d400;
  SDLST:Word absolute $230;
  CHBASE:Byte absolute $2f4;
  SOUNDR:Byte absolute $41;

procedure setColors;
var p:pointer;

begin
  p:=@colors[byte((playerStatus and ps_colorSet)=ps_colorSet),0];
  move(p,pointer($d016),5);
  move(p,pointer(708),5);
end;

procedure invers(chars:byte); Register; Assembler;
asm
  icl 'asms/invers.a65'
end;

procedure putSpaces(spaces:byte); Register; Assembler;
asm
  icl 'asms/put_spaces.a65'
end;

procedure putINTText(s:PString); Register; Assembler;
asm
  icl 'asms/putinttext.a65'
end;

procedure PutASCText(s:PString); Register; Assembler;
asm
  icl 'asms/putasctext.a65'
end;

procedure PutHex(var v; n:byte); Assembler;
asm
  icl 'asms/puthex.a65'
end;

procedure putInt(value:smallint); assembler;
asm
  icl 'asms/int2str.a65'
end;

procedure reset_MIDI;
var
  i:byte;

begin
  i:=0;
  while i<6 do
  begin
    FIFO_Byte:=GM_Reset[i]; FIFO_WriteByte;
    inc(i);
  end;
  FIFO_Flush;
end;

procedure stopMIDI;
begin
  _timerStatus:=_timerStatus or f_counter;
  _totalTicks:=0; _subCnt:=1;
  setIntVec(iTim1,oldTimerVec);
  Reset_MIDI;
end;

procedure hline; assembler;
asm
  icl 'asms/hline.a65'
end;

procedure clearWorkArea; assembler;
asm
  icl 'asms/clear_workarea.a65'
end;

procedure resetMemoryPtr;
begin
  last_bank:=totalXMS;
  if totalXMS>0 then
    last_adr:=$4000
  else
    last_adr:=$d800;
end;

procedure clearStatus;
var
  l:byte;
  oscradr,memAvailable:word;

begin
  fillchar(pointer(SCREEN_TIME),60,0);
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$fd);
  statusLoop;
  scradr:=SCREEN_STATUS; HLine;
  memAvailable:=(word(totalXMS)*16)+FREE_MEM;
  if memAvailable<10000 then scradr:=SCREEN_STATUS+29;
  if memAvailable<1000 then scradr:=SCREEN_STATUS+30;
  if memAvailable<100 then scradr:=SCREEN_STATUS+31;
  if memAvailable<10 then scradr:=SCREEN_STATUS+32;
  oscradr:=scradr;
  putINTText('Mem:'~); putInt(memAvailable); putINTText('KB'~);
  l:=length(fn);
  if l=0 then exit;
  if l>16 then fn[0]:=#16;
  scradr:=oscradr-l-1; putASCText(fn);
end;

procedure clearUVMeters;
begin
  if (playerStatus and ps_view=0) or firstTime then
  begin
    chn:=0;
    repeat
      scradr:=channelScrAdr[chn];
      asm
        ldy #3
        lda #$41
      VClr:
        sta (scradr),y
        dey
        bpl VClr
      end;
      inc(chn);
    until chn=16;
  end;
end;

procedure exit2DOS;
begin
  stopMIDI;
  Cls(0); Opn(0,12,0,'E:'); SOUNDR:=1;
end;