{$LIBRARYPATH midfile/}
{$DEFINE ROMOFF}
Uses
  MC6850,
  MIDI_FIFO,
  MIDFiles,
  Misc,
  CIO;
{$I-}

{$i type.inc}
{$i const.inc}

{$r player.rc}

var
  channelScrAdr:array[0..15] of word;
  playlistScrAdr:array[0..15] of word;
  playerStatus:Byte absolute $4A;
  totalXMS:Byte absolute $4B;
  scradr:Word absolute $D4;
  MCBaseAddr:Word absolute $D8;
  pls:^Byte absolute $DA;
  _tm:Byte absolute $14;
  otm:Byte;
  chn:Byte;
  YFile,shFile,curFile,totalFiles:Byte;
  v:shortint;
  // i,c:Byte;
  firstTime:Boolean = True;
  isStopped:Boolean = False;
  curdev:TDevString;
  fn:TFilename absolute $500;
  outstr:TFilename absolute $580;
  last_bank:Byte;
  last_adr:Word;

{$i myNMI.inc}
{$i helpers.inc}
{$i status.inc}
{$i init.inc}
{$i load.inc}
{$i inputline.inc}
{$i playlist_asm.inc}
{$i fileselect.inc}
{$i playlist.inc}

begin
  init;

  // if paramCount>0 then
  // begin
  //   fn:=paramStr(1);
  //   preparePlaylist;
  // end
  // else
  begin
    joinStrings(curDev,'*.*');
    gotoNEntry(0);
    _adr:=$ffff; _bank:=$fe; addToPlaylist(outStr);
    choicePlaylistFile;
    // fileSelect(outStr);
  end;

  setNMI;

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
      scradr:=screen_time+11; putHex(@_totalTicks,8);

      asm  icl 'asms/uvmeters.a65' end;
    end;

    {$i keyboard.inc}
  until false;

// End player loop
  unsetNMI;

  exit2DOS;
end.