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
  playerStatus:Byte absolute $4A;
  totalXMS:Byte absolute $4B;
  scradr:Word absolute $D4;
  MCBaseAddr:Word absolute $D8;
  _tm:Byte absolute $14;
  otm:Byte absolute $13;
  ctm:Byte absolute $12;
  chn:Byte absolute $D6;
  v:shortint absolute $D7;

  fileList:Pointer absolute $DA;
  YFile:Byte absolute $400;
  shFile:Byte absolute $401;
  curFile:Byte absolute $402;
  totalFiles:Byte absolute $403;

  curPlay:Byte absolute $404;
  playDir:Byte absolute $405;

  last_bank:Byte absolute $40a;
  last_adr:Word absolute $40b;

  playlistScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  curdev:TDevString absolute $4fc;
  fn:TFilename absolute $500;
  outstr:TFilename absolute $580;

  ilch:Byte absolute $D6;
  ilpos:Byte absolute $450;
  ilscradr:Word absolute $451;
  ilvcrs:Boolean absolute $453;
  resultInputLine:boolean;
  stateInputLine:Byte;

{$i screen.inc}
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
    shFile:=0; YFile:=0; curFile:=0;
    choiceListFile; stateInputLine:=2; resultInputLine:=true; keyb:=k_RETURN;
  end;

  setNMI;

// Player loop
  Repeat
    processMIDI;
    if (playerStatus and ps_isStopped=0) and (playingTracks=0) then
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
              if p_bank=$ff then
                IOResult:=loadSong(outStr)
              else
                continue;
            until IOResult<>1;
            clearStatus;
            if IOResult and %11111100<>0 then statusError(IOResult);
            if totalTracks<>0 then
              statusPlaying;
            playDir:=1;
          end;
      end;
    end;

    if _tm<>otm then
    begin
      otm:=_tm;
      if playerStatus and ps_isRefresh<>0 then
      begin
        playerStatus:=playerStatus xor ps_isRefresh;
        showList;
        drawListSelection;
      end;
      scradr:=screen_time+12; putHex(@_totalTicks,8);

      asm  icl 'asms/uvmeters.a65' end;
      if stateInputLine=1 then
        if _tm-ctm>10 then
        begin
          ctm:=_tm;
          ilvcrs:=not ilvcrs;
          show_inputLine;
        end;

    end;

    {$i keyboard.inc}
  until false;

// End player loop
  unsetNMI;

  exit2DOS;
end.