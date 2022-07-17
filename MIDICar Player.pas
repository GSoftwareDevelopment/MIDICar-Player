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
  _v:byte absolute $D7;
  memAvailable:longint;

  fileList:Pointer absolute $DA;
  YFile:Byte;// absolute $400;
  shFile:Byte;// absolute $401;
  curFile:Byte;// absolute $402;
  totalFiles:Byte;// absolute $403;

  curPlay:Byte;// absolute $404;
  playDir:Byte;// absolute $405;

  last_bank:Byte;// absolute $406;
  last_adr:Word;// absolute $407;

  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  curdev:TDevString;// absolute $4fc;
  fn:TFilename absolute $500;
  outstr:TFilename absolute $580;

  ilch:Byte absolute $D6;
  ilpos:Byte;// absolute $450;
  ilscradr:Word;// absolute $451;
  ilvcrs:Boolean;// absolute $453;
  resultInputLine:boolean;
  stateInputLine:Byte;
  counter:Longint absolute $88;
  cntBCD:Longint absolute $8c;

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
{$i keyboard.inc}
{$i autostop_songchange.inc}
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
    AutoStopAndSongChange;

    if _tm<>otm then
    begin
      otm:=_tm;
      if playerStatus and ps_isRefresh<>0 then
      begin
        playerStatus:=playerStatus xor ps_isRefresh;
        showList;
        drawListSelection;
      end;
      counter:=_totalTicks;
      _v:=(counter div _songTicks)-1;
      if v<>255 then
      begin
      asm
        txa
        sta oldx

        lda v
        and #%11
        tax
        lda v
        lsr @
        lsr @
        tay

        lda progressData,x
        sta screen_timeline,y

        lda oldx:#00
        bpl skipData ; always jump
      progressData:
        .byte %01000000
        .byte %01010000
        .byte %01010100
        .byte %01010101
      skipData:
      end;
      end;

      asm
        icl 'asms/24bin_6bcd.a65'
      end;
      scradr:=screen_time+14; putHex(cntBCD,6);

      asm  icl 'asms/uvmeters.a65' end;
      if stateInputLine=1 then
        if _tm-ctm>10 then
        begin
          ctm:=_tm;
          ilvcrs:=not ilvcrs;
          show_inputLine;
        end;

    end;

    if (keyb<>255) or (hlpflg<>0) then
    begin
      if playerStatus and ps_isHelp<>0 then toggleHelpScreen;
      if stateInputLine=1 then
        do_inputLine
      else
      begin
        if (keyb=k_H) or (hlpflg=k_HELP) then toggleHelpScreen
        else if keyb=k_ESC then break
        else if keyb=k_SPACE then toggleFile
        else if (keyb=k_UP) or (keyb=k_DOWN) then moveFileSelection
        else if keyb=k_L then toggleLoopMode
        else if keyb=k_M then toggleMeters
        else if keyb=k_INVERS then toggleScreenColors;
        playerControl;
        if (keyb=k_CLEAR) or (keyb=k_INSERT) or (keyb=k_DELETE) then tempoControl;
      end;

      if keyb=k_RETURN then fileAction;

      keyb:=255;
      hlpflg:=0;
    end;

  until false;

// End player loop
  unsetNMI;

  exit2DOS;
end.