procedure resetMemoryPtr;
begin
  memAvailable:=(word(totalXMS)*16384)+FREE_MEM;
  last_bank:=totalXMS;
  if totalXMS>0 then
    last_adr:=$4000
  else
    last_adr:=$d800;
end;

procedure clearUVMeters; Assembler;
asm
  icl 'asms/uvmeters_clear_h.a65'
end;

procedure statusLoop;
const
  sloop:Array[0..3] of string[2] = ( {?}
    #$00+#$00,  // sloop_playone
    #$7e+#$7f,  // sloop_repeatone
    #$5c+#$5d,  // sloop_repeatall
    #$5e+#$5f   // sloop_shuffle
  );

begin
  scradr:=SCREEN_TIME+6;
  v:=playerStatus and ps_loop;
  putINTText(sloop[v]);
end;

procedure statusPlaying;
begin
  scradr:=SCREEN_TIME;
  if totalTracks>0 then
  begin
    putINTText(#$3b+#0+#$fc+#0+#$fd);
    scradr:=SCREEN_STATUS; PutINTText(#$4e+'Playing'*~+#$ce);
    if (playerStatus and ps_isStopped)<>0 then
    begin
      initMIDI;
      playerStatus:=playerStatus xor ps_isStopped;
    end
    else
      _timerStatus:=_timerStatus and not f_counter;
  end
  else
  begin
    putINTText(#$fb+#0+#$fc+#0+#$fd);
    scradr:=SCREEN_STATUS; PutINTText(#$4e+'No song'*~+#$ce);
  end;
end;

procedure statusPause;
begin
  if (playerStatus and ps_isStopped)<>0 then exit;
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$3c+#0+#$fd);
  scradr:=SCREEN_STATUS; PutINTText(#$4e+' Pause '*~+#$ce);
  _timerStatus:=_timerStatus or f_counter;
end;

procedure statusStopped;
begin
  fillchar(pointer(SCREEN_TIMELINE),40,0);
  if totalTracks=0 then exit;
  if (playerStatus and ps_isStopped)<>0 then exit;
  playerStatus:=playerStatus or ps_isStopped;
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$3d);
  scradr:=SCREEN_STATUS; PutINTText(#$4e+'Stopped'*~+#$ce);
  doneMIDI;
  move(pointer(START_INFO_ADDR),pointer(TRACK_DATA_ADDR),512);
  chn:=0;
  clearUVMeters;
end;

procedure statusLoading;
begin
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Loading '*~);
end;

procedure statusCalcLength;
begin
  scradr:=SCREEN_STATUS; PutINTText(#$4e+'Calculating length'*~+#$ce);
end;

procedure statusReadDirectory;
begin
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Reading directory'*~+#$ce);
end;

// procedure statusReadPlaylist;
// begin
//   scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Reading playlist'*~+#$ce);
// end;

procedure updateMem;
var
  memAKB:Word;  {?}

begin
  scradr:=SCREEN_FOOT; HLine;
  scradr:=SCREEN_FOOT; putINTText(VERSION);
  memAKB:=memAvailable div 1024;
  scradr:=SCREEN_FOOT+33;
  if memAKB<10000 then scradr:=SCREEN_FOOT+34;
  if memAKB<1000 then scradr:=SCREEN_FOOT+35;
  if memAKB<100 then scradr:=SCREEN_FOOT+36;
  if memAKB<10 then scradr:=SCREEN_FOOT+37;
  {putINTText(#$4a);} putInt(memAKB); putINTText('KB'~);
end;

procedure clearStatus;
// var
//   l:byte;

begin
  fillchar(pointer(SCREEN_TIME),100,0);
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$fd);
  statusLoop;
  updateMem;
  asm
    lda main.filestr.adr.fn
    beq @exit
    cmp #16
    bmi skipTrim
    beq skipTrim

    lda #16
    sta main.filestr.adr.fn
  skipTrim:

    sta l
    lda #<(SCREEN_STATUS+40)
    sub l:#00
    sta screen.scradr
    lda #>(SCREEN_STATUS+40)
    sbc #0
    sta screen.scradr+1
  end;
  putASCText(fn);
end;

procedure statusError(err:byte);
begin
  keyb:=255;
  scradr:=SCREEN_STATUS+1;
  case err of
    ERR_UNSUPPORTED_FORMAT: PutINTText(#$4e+'Unsupported format'~*+#$ce);
    ERR_NOT_ENOUGHT_MEMORY: PutINTText(#$4e+'Not enought memory'~*+#$ce);
    ERR_NOT_A_MIDI_FILE:    PutINTText(#$4e+'Not a MIDI file'~*+#$ce);
    ERR_FULL_LIST:          PutINTText(#$4e+'List is full'~*+#$ce);
  else
    puttextinvert:=128;
    PutINTText(#$4e+'I/O Error #'*~); putInt(err); poke(scradr,$ce);
    puttextinvert:=0;
  end;
  while keyb=255 do ;
  clearStatus;
end;
