{$LIBRARYPATH './midfile/'}
{$LIBRARYPATH './units/'}
{$LIBRARYPATH './includes/'}
{$DEFINE ROMOFF}
Uses
  MIDFiles,
  keys,
  CIO,
  filestr,
  screen,
  list,
  inputline;
{$I-}

var PORTB:byte absolute $D301;

{$i 'macros.inc'}

{$i 'const.inc'}

{$r 'player.rc'}

var
  _tm:Byte absolute $14;
  otm:Byte absolute $13;
  ctm:Byte absolute $12;

  OSDTm:Byte absolute $D4;
  chn:Byte absolute $D5;
  oldv:byte absolute $D6;
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

  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  lstY:Byte absolute $5A;
  lstShift:SmallInt absolute $5B;
  lstCurrent:SmallInt absolute $70;
  lstTotal:SmallInt absolute $74;

  curPlay:SmallInt absolute $72;

// counter

  timeShowMode:Byte = 1;

  counter:Longint absolute $88;
  cntBCD:Longint absolute $8c;

//

{$i 'myNMI.inc'}
{$i 'NRPM.inc'}
{$i 'osd.inc'}
{$i 'status.inc'}
{$i 'load.inc'}
{$i 'list.inc'}
procedure fileTypeByExt(s:PString); Forward;
{$i 'playlist.inc'}
{$i 'getdirectory.inc'}
{$i 'keyboard.inc'}
{$i 'autostop_songchange.inc'}
{$i 'init.inc'}
{$i 'remote_control.inc'}

begin
  asm lda PORTB \ pha end;
  init;

// Loader init `outstr` and now its time to check for correct file specification
  validPath; // result in '_v' indicate what part of file spec is available
  if isBitClr(_v,dp_name) then // if file name is not specified...
    createListEntry(fl_device,outStr) // create Device entry type
  else
  begin
    fileTypeByExt(fn);
    createListEntry(p_type,fn);
    if p_type=fl_midifile then toggleMinMode;
  end;

  choiceListFile; resultInputLine:=true; keyb:=k_RETURN;

// Player loop
  Repeat
    processMIDI;
    AutoStopAndSongChange;

    // plugin_remoteControl;

    if timer(otm,refreshRate) then
    begin
      resetTimer(otm);

      if isBitSet(screenStatus,ss_isOSD) then
      begin
        if OSDTm>200 then OSDOff else inc(OSDTm,refreshRate);
      end;

      if isBitSet(screenStatus,ss_isRefresh) then
      begin
        clrBit(screenStatus,ss_isRefresh);
        showList;
        drawListSelection;
      end;

      counter:=_totalTicks;
      if isBitSet(playerStatus,ps_calcLen) and (_songTicks>0) then
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

      if isBitSet(stateInputLine,ils_inprogress) then
        if timer(ctm,10) then
        begin
          resetTimer(ctm);
          show_inputLine;
        end;

    end;

    if (keyb<>255) then
    begin
      if isBitSet(screenStatus,ss_isHelp) then toggleHelpScreen;

      if stateInputLine=ils_inprogress then do_inputLine;

      asm
        lda MAIN.KEYS.keyb
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
    end;

  until false;

// End player loop
end.