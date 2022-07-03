// const
//   GM_RESET: array[0..5] of byte = ($f0, $7e, $7f, $09, $01, $f7);

{$i keys.inc}

var
  keyb:byte absolute $2fc;
  hlpflg:byte absolute $2dc;

  SOUNDR:Byte absolute $41;


procedure reset_MIDI; assembler;
asm
  txa:pha
  ldx #0
sendData:
  lda GM_RESET,x
  sta MAIN.MIDI_FIFO.FIFO_Byte
  jsr MAIN.MIDI_FIFO.FIFO_WriteByte
  inx
  cpx #6
  bne sendData
  jsr MAIN.MIDI_FIFO.FIFO_Flush
  pla:tax
  rts
GM_RESET:
  .byte $f0, $7e, $7f, $09, $01, $f7
end;

procedure stopMIDI;
begin
  _timerStatus:=_timerStatus or f_counter;
  _totalTicks:=0; _subCnt:=1;
  setIntVec(iTim1,oldTimerVec);
  Reset_MIDI;
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
  oscradr:word;
  memAvailable:word;

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

procedure clearUVMeters; Assembler;
asm
  icl 'asms/uvmeters_clear.a65'
end;

procedure exit2DOS;
begin
  stopMIDI;
  Cls(0); Opn(0,12,0,'E:'); SOUNDR:=1;
end;