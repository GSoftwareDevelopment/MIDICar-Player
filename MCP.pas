{$LIBRARYPATH midfile/}
{$LIBRARYPATH units/}
{$LIBRARYPATH includes/}
{$DEFINE ROMOFF}
Uses
  MIDFiles,
  Misc,
  CIO,
  keys,
  filestr,
  screen,
  list;
{$I-}

{$i const.inc}
{$i type.inc}

{$r player.rc}

var
  _tm:Byte absolute $14;
  otm:Byte absolute $13;
  ctm:Byte absolute $12;
  refreshRate:Byte absolute $D2;
  totalXMS:Byte absolute $D3; // this value is initialized by loader
  scradr:Word absolute $D4;
  chn:Byte absolute $D6;
  v:shortint absolute $D7;
  _v:byte absolute $D7;
  playerStatus:Byte absolute $D8;
  screenStatus:Byte absolute $D9;
  memAvailable:longint;

  lstY:Byte = 0;
  lstShift:SmallInt = 0;
  lstCurrent:SmallInt = 0;
  lstTotal:SmallInt;

  curPlay:SmallInt;

  last_bank:Byte;
  last_adr:Word;

  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  curdev:TDevString absolute CURDEV_ADDR;   // 6 (+1) // absolute $4f0;
  curPath:TPath absolute CURPATH_ADDR;      // 64 (+1)
  fn:TFilename absolute FN_PATH_ADDR;       // 32 (+1)
  outstr:String[80] absolute OUTSTR_ADDR;   // 80 (+1)
  Snull:String[80] absolute SNULL_ADDR;     // 80 (+1)

  ilch:Byte absolute $D6;
  ilpos:Byte absolute $54;
  ilscradr:Word absolute $55;

  resultInputLine:boolean = false;
  stateInputLine:Byte = 0;

  timeShowMode:Byte = 1;

  counter:Longint absolute $88;
  cntBCD:Longint absolute $8c;

procedure statusLoop; Forward;
procedure drawListSelection; Forward;
{$i myNMI.inc}
{$i helpers.inc}
{$i status.inc}
{$i load.inc}
{$i inputline.inc}
{$i list.inc}
{$i getdirectory.inc}
{$i keyboard.inc}
{$i autostop_songchange.inc}
{$i init.inc}

begin
  asm lda PORTB \ pha end;
  init;

  outStr:='D:'; validPath;

  gotoNEntry(0); _adr:=$ffff; _bank:=fl_device; addToList(outStr);
  choiceListFile; stateInputLine:=ils_done; resultInputLine:=true; keyb:=k_RETURN;

  setNMI;

// Player loop
  Repeat
    processMIDI;
    AutoStopAndSongChange;

    if _tm-otm>refreshRate then
    begin
      otm:=_tm;
      if screenStatus and ss_isRefresh<>0 then
      begin
        screenStatus:=screenStatus xor ss_isRefresh;
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

      if stateInputLine=ils_inprogress then
        if _tm-ctm>10 then
        begin
          ctm:=_tm;
          show_inputLine;
        end;

    end;

    if (keyb<>255) or (hlpflg<>0) then
    begin
      if screenStatus and ss_isHelp<>0 then toggleHelpScreen;
      if stateInputLine=ils_inprogress then
        do_inputLine
      else
      begin
        if keyb=k_ESC then break;

        asm
          lda MAIN.KEYS.hlpflg
          seq:sta MAIN.KEYS.keyb

          lda MAIN.KEYS.keyb
          tay
          and #%11000000
          sta MAIN.KEYS.keymod
          tya
          and #%00111111
          sta MAIN.KEYS.keyb

          asl @
          tay

          lda KEY_TABLE_ADDR+1,y
          beq key_not_defined
          sta jump_addr+1
          lda KEY_TABLE_ADDR,y
          sta jump_addr

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
  asm pla \ sta PORTB end;
end.