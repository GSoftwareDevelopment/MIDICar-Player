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
  SCREEN_ADDR     = $3422;
  SCREEN_TIME     = SCREEN_ADDR+23*40;
  SCREEN_STATUS   = SCREEN_TIME+20;
  UVMETER_ADDR    = $3880;

  START_INFO_ADDR = $3C00;
  TRACK_DATA_ADDR = $3E00;
  MIDI_DATA_ADDR  = $4000;
  FREE_MEM        = (($8000-$4000)+($d000-$a400)+($ff00-$e000)) div 1024;

  f_clear = %00100000;

  ps_colorSet = %001;
  ps_view     = %010;
  ps_loop     = %100;

{$r player.rc}
{$r selftest.rc}

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

{$i helpers.inc}

{$i init.inc}
{$i load.inc}
{$i fileselect.inc}

begin
  init;

  if paramCount=1 then
  begin
    fn:=ParamStr(1);
    loadSong;
    clearStatus;
  end
  else
  begin
    // fileSelect;
    totalTracks:=0;
  end;
//
  clearUVMeters;
  statusPlaying;

  // initialize MIDICar (MC6850)
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+IRQ_Receive);

  Reset_MIDI;        // reset MIDI
  initTimer;

// Player loop

  Repeat
    processMIDI;
    if not isStopped and (playingTracks=0) then
    begin
      if playerStatus and ps_loop<>0 then
      begin
        statusStopped;
        statusPlaying;
      end
      else
        keyb:=k_s;
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

  statusStopped;
  NMIEN:=$00; NMIVEC:=oldNMIVec; NMIEN:=$40;
  exit2DOS;
end.