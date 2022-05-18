unit MIDFiles;

interface

{$i types.inc}
{$i consts.inc}

var
// Zero page work registers
  _totalTicks:TDeltaVar   absolute $f0; {f0, f1, f2, f3}
  _subCnt:Byte            absolute $f4;
  _timerStatus:Byte       absolute $f5;
  _delta:Longint          absolute $f6; {f7, f8, f9, fa}
  _tmp:Byte               absolute $f6; // must by the same address as _delta!!

// the order of the registers MUST be the same as in the TMIDTrack record!!!
  _bank:Byte              absolute _trkRegs;
  _ptr:^Byte              absolute _trkRegs+1;   {16-bits}
  _adr:Word               absolute _trkRegs+1;   // must be the same address as _ptr!!
  _trackTime:TDeltaVar    absolute _trkRegs+3; {32-bits}
  _status:Byte            absolute _trkRegs+7;
  _event:Byte             absolute _trkRegs+8;

//
  MIDData:Pointer;
  MIDTracks:TByteArray;
  format:byte;
  totalTracks:byte;
  tickDiv:Word;
  ms_per_qnote:longint;
// The following variables are informational only
// they are not used in the file playback process
{$IFDEF USE_SUPPORT_VARS}
  fps:Byte;
  fsd:Byte;
  tactNum:Byte;
  tactDenum:Byte;
  ticks_per_qnote:Byte;
  ticks_per_32nd:Byte;
  BPM:Word;
{$ENDIF}
  oldTimerVec:Pointer;

  curTrackPtr:Pointer;
  deltaTime:TDeltaVar ;
  dTm:word;
  cTrk,PlayingTracks:Byte;

  loadProcess:TLoadingProcess;

//
function LoadMID(fn:PString):shortint;
procedure ProcessMIDI;
procedure ProcessTrack;
procedure setTempo;

implementation
Uses
  {$IFDEF USE_FIFO}MIDI_FIFO,{$ENDIF}
  MC6850;

var
  // BI:TBigIndian;
  RBuf:TByteArray absolute $600;

{$i int_timer.inc}
{$i memboundcheck.inc}

{$i loadmid.inc}
{$i processtrack.inc}
{$i processmidi.inc}
{$i settempo.inc}

procedure nullLoadPrcs;
begin
end;

initialization
  oldTimerVec:=nil;
  loadProcess:=@nullLoadPrcs;
  tickDiv:=384;
  ms_per_qnote:=500000;
{$IFDEF USE_SUPPORT_VARS}
  tactNum:=4;
  tactDenum:=4;
  ticks_per_qnote:=24;
  ticks_per_32nd:=8;
{$ENDIF}
  _timerStatus:=0;
  getIntVec(iTim1,oldTimerVec);
end.