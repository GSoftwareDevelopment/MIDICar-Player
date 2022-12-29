{$LIBRARYPATH midfile/}
{$LIBRARYPATH units/}
{$LIBRARYPATH includes/}
{$DEFINE ROMOFF}
Uses
  MIDFiles,
  keys,
  CIO,
  filestr,
  screen,
  list,
  inputLine;
{$I-}

{$i const.inc}

{$r player.rc}

var
  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;
  _tm:Byte absolute $14;
  otm:Byte absolute $13;
  ctm:Byte absolute $12;
  chn:Byte absolute $D6;
  v:shortint absolute $D7;
  _v:byte absolute $D7;
  playerStatus:Byte absolute $D8;
  screenStatus:Byte absolute $D9;

// this value is initialized by loader
  refreshRate:Byte absolute $D2;
  totalXMS:Byte absolute $D3;

//
  memAvailable:longint;

// selector variables

  lstY:Byte;
  lstShift:SmallInt;
  lstCurrent:SmallInt;
  lstTotal:SmallInt;

  curPlay:SmallInt;

  last_bank:Byte;
  last_adr:Word;

// counter

  timeShowMode:Byte = 1;

  counter:Longint absolute $88;
  cntBCD:Longint absolute $8c;

procedure drawListSelection; Forward;
{$i myNMI.inc}
{$i status.inc}
{$i load.inc}
{$i list.inc}
{$i getdirectory.inc}
{$i keyboard.inc}
{$i autostop_songchange.inc}
{$i init.inc}

begin
  asm lda PORTB \ pha end;
  init;

// Loader init `outstr` and now its time to check for correct file specification
  validPath; // result in '_v' indicate what part of file spec is available
  _adr:=$ffff;
  if (_v and dp_name=0) then // if file name is not specified...
    _bank:=fl_device // set entry type as Device
  else
    _bank:=fl_midifile; // set entry type as MIDIFila

  // create new entry
  gotoNEntry(0); addToList(outStr);
  choiceListFile; resultInputLine:=true; keyb:=k_RETURN;

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
      if (playerStatus and ps_calcLen<>0) and (_songTicks>0) then
      begin
        _v:=(counter div _songTicks)-1;
        if _v<>255 then
        asm
          icl 'asms/song_progress.a65'
        end;
      end;

      scradr:=screen_time+54;
      if timeShowMode=1 then
        putHex(counter,6)
      else if timeShowMode=2 then
      begin
        asm
          icl 'asms/24bin_6bcd.a65'
        end;
        putHex(cntBCD,6);
      end;

      asm  icl 'asms/uvmeters_h.a65' end;

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
      if stateInputLine=ils_inprogress then do_inputLine;

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

      keyb:=255;
      hlpflg:=0;
    end;

  until false;

// End player loop
end.