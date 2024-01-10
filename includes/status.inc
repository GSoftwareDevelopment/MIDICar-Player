{$i statuses.inc}

procedure putStatus(msg:PString); Register;
begin
  scradr:=SCREEN_STATUS; PutINTText(msg);
end;

procedure calcAvailableMem; Assembler;
asm // totalXMS * 16384
  ldy totalXMS
  lda #0
  sta memAvailable+3
  sty memAvailable+2
  sta memAvailable+1
  sta memAvailable+0
  clc
  ror memAvailable+2
  ror memAvailable+1
  ror memAvailable+2
  ror memAvailable+1
end;

procedure clearUVMeters; Assembler;
asm
  icl 'asms/uvmeters_clear_h.a65'
end;

procedure statusLoop;
begin
  scradr:=SCREEN_TIME+46;
  v:=playerStatus and ps_loop;
  putINTText(sloop[v]);
end;

procedure statusPlaying;
begin
  scradr:=SCREEN_TIME+40;
  if totalTracks>0 then
  begin
    putINTText(controls[1]); // play sign
    putStatus(statusMsgs[0]);
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
    putINTText(controls[0]); // play pause stop
    putStatus(statusMsgs[1]);
  end;
end;

procedure statusPause;
begin
  if (playerStatus and ps_isStopped)<>0 then exit;
  scradr:=SCREEN_TIME+40; putINTText(controls[2]);
  putStatus(statusMsgs[2]);
  _timerStatus:=_timerStatus or f_counter;
end;

procedure statusStopped;
begin
  fillchar(pointer(SCREEN_TIMELINE),40,0);
  if (totalTracks=0) or
     ((playerStatus and ps_isStopped)<>0) then exit;
  playerStatus:=playerStatus or ps_isStopped;
  scradr:=SCREEN_TIME+40; putINTText(controls[3]);
  putStatus(statusMsgs[3]);
  doneMIDI;
  asm
    lda PORTB
    pha
    ldy totalXMS
    lda $100,y
    sta PORTB
  end;
  move(pointer(START_INFO_ADDR),pointer(TRACK_DATA_ADDR),512);
  asm
    pla
    sta PORTB
  end;
  chn:=0;
  clearUVMeters;
end;

function getver():pstring;
begin
  result:={$I %DATE%};
end;

procedure updateMem;
var
  memAKB:Word absolute $8c; {?}

begin
  scradr:=SCREEN_FOOT; HLine;
  scradr:=SCREEN_FOOT; putINTText(VERSION); putASCText(getver());
  inc(scradr);
  asm // put MIDI Driver Name
    ldy #0

  loop:
    lda $2012,y
    cmp #$20
    beq stopLoop

    asl
    adc #$c0
    spl:eor #$40
    lsr
    scc:eor #$80

    sta (main.screen.scradr),y
    iny
    jmp loop
  stopLoop:
  end;

  asm // memAvailable div 1024
    ldy memAvailable+1
    sty memAKB+0
    lda memAvailable+2
    sta memAKB+1
    tya
    clc
    ror memAKB+1  // 8192
    ror memAKB+0
    clc
    ror memAKB+1  // 8192
    ror memAKB+0
  end;
  scradr:=SCREEN_FOOT+33;
  if memAKB<10000 then scradr:=SCREEN_FOOT+34;
  if memAKB<1000 then scradr:=SCREEN_FOOT+35;
  if memAKB<100 then scradr:=SCREEN_FOOT+36;
  if memAKB<10 then scradr:=SCREEN_FOOT+37;
  putInt(memAKB); putINTText('KB'~);
end;

procedure updateTimeMode;
begin
  scradr:=screen_time+52; putSpaces(8);
  if timeShowMode>=3 then
  begin
    scradr:=screen_time+57; poke(scradr,$0d);
  end;
end;

procedure clearControls;
begin
//  fillchar(pointer(SCREEN_TIME+40),20,0);
  scradr:=SCREEN_TIME+40; putINTText(controls[0]);
  statusLoop;
  updateTimeMode;
end;

procedure clearStatus;
var
  memAKB:Word absolute $8c; {?}

begin
  fillchar(pointer(SCREEN_TIME+40),60,0);
  fillchar(pointer(SCREEN_STATUS),40,0);
  updateMem;

  if byte(fn[0])=0 then exit;
  asm // fileSize div 1024
    ldy MAIN.MIDFILES.fileSize+1
    sty memAKB+0
    lda MAIN.MIDFILES.fileSize+2
    sta memAKB+1
    tya
    clc
    ror memAKB+1  // 8192
    ror memAKB+0
    clc
    ror memAKB+1  // 8192
    ror memAKB+0
  end;
  if memAKB>0 then
  begin
    scradr:=SCREEN_STATUS+33;
    if memAKB<10000 then scradr:=SCREEN_STATUS+34;
    if memAKB<1000 then scradr:=SCREEN_STATUS+35;
    if memAKB<100 then scradr:=SCREEN_STATUS+36;
    if memAKB<10 then scradr:=SCREEN_STATUS+37;
    asm
      ldy MAIN.SCREEN.scradr
      dey
      tya
      pha
      lda MAIN.SCREEN.scradr+1
      pha
    end;
    putInt(memAKB); putINTText('KB'~);
  end
  else
  begin
    asm
      lda #<(SCREEN_STATUS+40)
      pha
      lda #>(SCREEN_STATUS+40)
      pha
    end;
  end;

  asm
    pla
    sta scraddrHi
    pla
    sta scraddrLo

    lda main.filestr.adr.fn
    cmp #16
    bmi skipTrim
    beq skipTrim

    lda #16
    sta main.filestr.adr.fn
  skipTrim:

    sta l
    lda scraddrLo:#<(SCREEN_STATUS+40)
    sub l:#00
    sta screen.scradr
    lda scraddrHi:#>(SCREEN_STATUS+40)
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
    ERR_UNSUPPORTED_FORMAT: putStatus(errorMsgs[1]);
    ERR_NOT_ENOUGHT_MEMORY: putStatus(errorMsgs[2]);
    ERR_NOT_A_MIDI_FILE:    putStatus(errorMsgs[3]);
    ERR_LIST_IS_FULL:       putStatus(errorMsgs[4]);
  else
    puttextinvert:=128;
    putStatus(errorMsgs[0]);
    putInt(err); poke(scradr,$ce);
    puttextinvert:=0;
  end;
  // while keyb=255 do ;
  IOResult:=0;
  // clearStatus;
end;
