{$LIBRARYPATH midfile/}
{$DEFINE ROMOFF}
Uses
  MC6850,
  MIDI_FIFO,
  MIDFiles,
  Misc,
  CIO;
{$I-}

const
  CHARS_ADDR      = $3000;
  DLIST_ADDR      = $3400;
  SCREEN_ADDR     = $3448;
  SCREEN_WORK     = SCREEN_ADDR+19*40;
  SCREEN_TIME     = SCREEN_WORK+17*40;
  SCREEN_STATUS   = SCREEN_TIME+20;
  SCREEN_FOOT     = SCREEN_STATUS+40;
  UVMETER_ADDR    = $2BC0;

  START_INFO_ADDR = $2C00;
  TRACK_DATA_ADDR = $2E00;
  MIDI_DATA_ADDR  = $4000;
  FREE_MEM        = (($8000-$4000)+($d000-$a800)+($ff00-$e000)) div 1024;

  f_clear = %00100000;

  ps_colorSet = %001;
  ps_view     = %010;
  ps_loop     = %100;

{$r player.rc}

var
  channelScrAdr:array[0..15] of word;
  scradr:Word absolute $d4;
  _tm:Byte absolute $14;
  otm:Byte;
  chn:Byte;
  TPtr:^Byte;
  TPS:Word;
  sec:Byte;
  TrkStat:Byte;
  v,i,c:Byte;
  isStopped:Boolean = False;
  fn:PString;
  oldNMIVec:Pointer;
  playerStatus:Byte absolute $4a;
  totalXMS:Byte;

{$i helpers.inc}
{$i status.inc}
{$i init.inc}
{$i load.inc}
{$i fileselect.inc}

begin
  init;

  if paramCount>0 then
  begin
    fn:=ParamStr(1);
    _bank:=totalXMS;
    _adr:=$4000;
    loadSong;
  end;

  setNMI;

  clearStatus;
  clearUVMeters;

// Player loop
  Repeat
    processMIDI;
    if not isStopped and (playingTracks=0) then
    begin
      statusStopped;
      if playerStatus and ps_loop<>0 then
        statusPlaying;
    end;

    if _tm<>otm then
    begin
      otm:=_tm;
      scradr:=screen_time+6; putHex(@_totalTicks,8);

      {$i uvmeters.inc}
    end;

    {$i keyboard.inc}
  until false;

// End player loop
  unsetNMI;

  exit2DOS;
end.