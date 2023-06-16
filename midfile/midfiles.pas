unit MIDFiles;

interface

{$i 'types.inc'}
{$i 'consts.inc'}

var
// Zero page work registers
  curTrackPtr:Pointer     absolute $dc;
  cTrk:Byte               absolute $de;
  PlayingTracks:Byte      absolute $df;

  _totalTicks:TDeltaVar   absolute $f0; {f0, f1, f2, f3}
  _subCnt:Byte            absolute $f4;
  _timerStatus:Byte       absolute $f5;
  _delta:Longint          absolute $f6; {f6, f7, f8, f9}
  _tmp:Byte               absolute $f6; // must by the same address as _delta!!

  _tickStep:Byte          absolute $e8;
  _songTicks:TDeltaVar    absolute $e9; {e9, ea, eb, ec}

// the order of the registers MUST be the same as in the TMIDTrack record!!!
  _bank:Byte              absolute _trkRegs;
  _ptr:^Byte              absolute _trkRegs+1;   {16-bits}
  _adr:Word               absolute _trkRegs+1;   // must be the same address as _ptr!!
  _trackTime:TDeltaVar    absolute _trkRegs+3;   {32-bits}
  _eStatusRun:Byte        absolute _trkRegs+7;

//
  MIDData:Pointer         ;
  MIDTracks:TByteArray    ;
  format:byte             ;
  totalTracks:byte        = 0;
  tickDiv:Word            = 384;
  us_per_qnote:longint    = 500000;
// The following variables are informational only
// they are not used in the file playback process
{$IFDEF USE_SUPPORT_VARS}
  fps:Byte                ;
  fsd:Byte                ;
  tactNum:Byte            = 4;
  tactDenum:Byte          = 4;
  ticks_per_qnote:Byte    = 24;
  ticks_per_32nd:Byte     = 8;
  BPM:Word                = 120;
{$ENDIF}
  chnVolume:Array[0..15] of byte;
  oldTimerVec:Pointer     ;

  loadProcess:TLoadingProcess;
  tempoShift:Longint      = 0;

//
function LoadMID:shortint;
procedure initMIDI;
procedure resetMIDI; assembler;
procedure doneMIDI;
procedure setTempo;
procedure ProcessTrack; Assembler;
procedure ProcessMIDI;
procedure determineSongLength;
procedure sendClearPushLCD; Keep;
procedure sendPushLCDText(str:PString); register; Keep;

implementation
Uses
  CIO;

procedure int_timer; Interrupt; Assembler;
asm
  icl 'midfile/asms/int_timer.a65'
end;

procedure memBoundCheck; Assembler;
asm
  icl 'midfile/asms/memory_bound_check.a65'
end;

{$i 'sysex_push_text.inc'}
{$i 'loadmid.inc'}
{$i 'settempo.inc'}
{$i 'processtrack.inc'}
{$i 'processmidi.inc'}
{$i 'determine_song_length.inc'}

procedure initMIDI;
begin
  _totalTicks:=0;    // reset song ticks
  tempoShift:=0;
  _timerStatus:=1;
  cTrk:=totalTracks;
  PlayingTracks:=totalTracks;

  asm
    sei
    mva <INT_TIMER VTIMR1
    mva >INT_TIMER VTIMR1+1
  // reset POKEY
    lda #$00
    ldy #$03
    sta AUDCTL
    sta AUDC1
    sty SKCTL
  // setup TIMER1
    sta AUDCTL
    mva 83 AUDF1
  // initialize IRQ for TIMER1
    lda irqens
    ora #$01
    sta irqens
    sta irqen
  // start timer strobe
    sta stimer

// call driver - setup
    jsr $2003

    cli  // enable IRQ
  end;
end;

procedure resetMIDI; assembler;
asm
GM_RESET_ADDR = $3B00;

  txa:pha

// call driver - setup
  jsr $2003

  ldx #0
  lda GM_RESET_ADDR-1,x
  beq skip_reset
  sta sysexlen

sendData:
  lda GM_RESET_ADDR,x

  ; call driver - Send
  jsr $2006

  inx
  cpx sysexlen:#6
  bne sendData

skip_reset:
  pla:tax

  rts

// GM_RESET:
  // .byte 6, $f0, $7e, $7f, $09, $01, $f7
  // .byte 7, $f0, $05, $7e, $7f, $09, $01, $f7
  // .byte 10, $f0, $08, $43, $10, $4c, $00, $00, $7e, $00, $f7
end;

procedure doneMIDI;
begin
  _timerStatus:=_timerStatus or f_counter;
  _totalTicks:=0; _subCnt:=0;
  setIntVec(iTim1,oldTimerVec);
  resetMIDI;
  asm
    sec       ; flush buffer & uninitialize driver
    jsr $200c
  end;
end;

procedure nullLoadPrcs;
begin
end;

initialization
  oldTimerVec:=nil;
  loadProcess:=@nullLoadPrcs;
  cTrk:=0;
  _songTicks:=0;
  _totalTicks:=0;
  _tickStep:=3;
  _timerStatus:=f_counter;
  getIntVec(iTim1,oldTimerVec);
end.