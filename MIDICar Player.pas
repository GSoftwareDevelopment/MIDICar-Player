{$LIBRARYPATH midfile/}
{$LIBRARYPATH units/}
{$LIBRARYPATH includes/}
{$DEFINE ROMOFF}
Uses
  MC6850,
  MIDI_FIFO,
  MIDFiles,
  Misc,
  CIO,
  keys,
  filestr,
  screen,
  list;
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

  YFile:Byte = 0;// absolute $400;
  shFile:Byte = 0 ;// absolute $401;
  curFile:Byte = 0;// absolute $402;
  totalFiles:Byte;// absolute $403;

  curPlay:Byte;// absolute $404;
  playDir:Byte = 1;// absolute $405;

  last_bank:Byte;// absolute $406;
  last_adr:Word;// absolute $407;

  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  curdev:TDevString;// absolute $4fc;
  fn:TFilename absolute $500;
  outstr:TFilename absolute $580;

  ilch:Byte absolute $D6;
  ilpos:Byte absolute $54;
  ilscradr:Word absolute $55;

  resultInputLine:boolean = false;
  stateInputLine:Byte = 0;

  timeShowMode:Byte = 1;

  counter:Longint absolute $88;
  cntBCD:Longint absolute $8c;

procedure statusLoop; Forward;
{$i myNMI.inc}
{$i helpers.inc}
{$i status.inc}
{$i load.inc}
{$i fileselect.inc}
{$i inputline.inc}
{$i list.inc}
{$i keyboard.inc}
{$i autostop_songchange.inc}
{$i init.inc}

begin
  init;

  joinStrings(curDev,'*.*');
  gotoNEntry(0);
  _adr:=$ffff; _bank:=$fe; addToList(outStr);
  choiceListFile; stateInputLine:=2; resultInputLine:=true; keyb:=k_RETURN;

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
      if _songTicks>0 then
      begin
        _v:=(counter div _songTicks)-1;
        if _v<>255 then
        asm
          icl 'asms/song_progress.a65'
        end;
      end;

      scradr:=screen_time+14;
      if timeShowMode=1 then
        putHex(counter,6)
      else if timeShowMode=2 then
      begin
        asm
          icl 'asms/24bin_6bcd.a65'
        end;
        putHex(cntBCD,6);
      end;

      asm  icl 'asms/uvmeters.a65' end;
      if stateInputLine=1 then
        if _tm-ctm>10 then
        begin
          ctm:=_tm;
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
        if keyb=k_ESC then break;

        asm
          lda MAIN.KEYS.hlpflg
          seq:sta MAIN.KEYS.keyb

          lda MAIN.KEYS.keyb
          and #%00111111
          asl @
          tay

          lda KEY_TABLE,y
          sta jump_addr
          lda KEY_TABLE+1,y
          sta jump_addr+1
          beq key_not_defined

          jsr jump_addr:$ffff
        key_not_defined:
        end;
      end;

      if keyb=k_RETURN then fileAction;

      keyb:=255;
      hlpflg:=0;
    end;

  until false;

// End player loop
  unsetNMI;

  exit2DOS;
  asm
    jmp endProg

    icl 'key_table.a65'

  endProg:
  end;
end.