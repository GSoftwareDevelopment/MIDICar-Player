unit MIDFiles;

interface

{$i types.inc}
{$i consts.inc}

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

  _songTicks:TDeltaVar    absolute $e9; {e9, ea, eb, ec}

// the order of the registers MUST be the same as in the TMIDTrack record!!!
  _status:Byte            absolute _trkRegs;
  _bank:Byte              absolute _trkRegs+1;
  _ptr:^Byte              absolute _trkRegs+2;   {16-bits}
  _adr:Word               absolute _trkRegs+2;   // must be the same address as _ptr!!
  _trackTime:TDeltaVar    absolute _trkRegs+4;   {32-bits}
  _event:Byte             absolute _trkRegs+8;

//
  MIDData:Pointer         ;
  MIDTracks:TByteArray    ;
  format:byte             ;
  totalTracks:byte        ;
  tickDiv:Word            ;
  ms_per_qnote:longint    ;
// The following variables are informational only
// they are not used in the file playback process
{$IFDEF USE_SUPPORT_VARS}
  fps:Byte                ;
  fsd:Byte                ;
  tactNum:Byte            ;
  tactDenum:Byte          ;
  ticks_per_qnote:Byte    ;
  ticks_per_32nd:Byte     ;
  BPM:Word                ;
{$ENDIF}
  chnVolume:Array[0..15] of byte;
  oldTimerVec:Pointer     ;

  loadProcess:TLoadingProcess ;
  tempoShift:Longint      ;

//
function LoadMID(fn:PString):shortint;
procedure initTimer;
procedure setTempo;
procedure ProcessTrack; // Keep;
procedure ProcessMIDI;
procedure determineSongLength;

implementation
Uses
  CIO,
  {$IFDEF USE_FIFO}MIDI_FIFO,{$ENDIF}
  MC6850;

procedure int_timer; Interrupt; Assembler;
asm
  icl 'midfile/asms/int_timer.a65'
end;

procedure memBoundCheck; Assembler;
asm
  icl 'midfile/asms/memory_bound_check.a65'
end;

{$i loadmid.inc}
{$i settempo.inc}
{$i processtrack.inc}
{$i processmidi.inc}
{$i determine_song_length.inc}

procedure initTimer;
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

    cli  // enable IRQ
  end;
end;

procedure nullLoadPrcs;
begin
end;

initialization
  oldTimerVec:=nil;
  loadProcess:=@nullLoadPrcs;
  tickDiv:=384;
  ms_per_qnote:=500000;
  totalTracks:=0;
  cTrk:=0;
  _timerStatus:=f_counter;
{$IFDEF USE_SUPPORT_VARS}
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
{$ENDIF}
  getIntVec(iTim1,oldTimerVec);
end.