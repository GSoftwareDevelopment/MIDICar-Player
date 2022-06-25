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
  pls:Pointer absolute $DA;
  _tm:Byte absolute $14;
  otm:Byte;
  chn:Byte;
  YFile,shFile,curFile,totalFiles:Byte;
  curPlay,playDir:Byte;
  v:shortint;
  // i,c:Byte;
  firstTime:Boolean = True;
  isStopped:Boolean = False;
  isHelp:Boolean = False;
  curdev:TDevString;
  fn:TFilename absolute $500;
  outstr:TFilename absolute $580;
  last_bank:Byte;
  last_adr:Word;

procedure statusLoop; Forward;
{$i filestr.inc}
{$i myNMI.inc}
{$i helpers.inc}
{$i status.inc}
{$i load.inc}
{$i list_asm.inc}
{$i fileselect.inc}
{$i inputline.inc}
{$i list.inc}

{$i init.inc}
// {$i playlist.inc}
{$i help.inc}

begin
  init;
  clearUVMeters;

  // if paramCount>0 then
  // begin
  //   fn:=paramStr(1);
  //   preparePlaylist;
  // end
  // else
  begin
    joinStrings(curDev,'*.*');
    gotoNEntry(0);
    _adr:=$ffff; _bank:=$fe; addToList(outStr);
    choiceListFile;
  end;

  setNMI;

// Player loop
  Repeat
    processMIDI;
    if not isStopped and (playingTracks=0) then
    begin
      v:=playerStatus and ps_loop;
      statusStopped;
      if v<>ps_playonce then
      begin
        if v>ps_repeatone then
          if curPlay<>255 then
          begin
            repeat
              if curPlay=255 then curPlay:=curFile;
              if (v=ps_shuffle) then
                curPlay:=random(totalFiles)
              else
              begin
                inc(curPlay,playDir);
                if (curPlay=1) then curPlay:=totalFiles;
                if (curPlay=totalFiles) then curPlay:=1;
              end;
              curFile:=curPlay;
              choiceListFile;
            until curPlay<>255;
            playDir:=1;
          end;
        if totalTracks<>0 then statusPlaying;
      end;
    end;

    if _tm<>otm then
    begin
      otm:=_tm;
      scradr:=screen_time+12; putHex(@_totalTicks,8);

      asm  icl 'asms/uvmeters.a65' end;
    end;

    {$i keyboard.inc}
  until false;

// End player loop
  unsetNMI;

  exit2DOS;
end.