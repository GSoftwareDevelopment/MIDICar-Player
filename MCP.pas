{$LIBRARYPATH './midfile/'}
{$LIBRARYPATH './units/'}
{$LIBRARYPATH './includes/'}
{$DEFINE ROMOFF}
{$DEFINE NOROMFONT}
{$DEFINE IRQPATCH}
{$DEFINE DISABLEIOCBCOPY}
Uses
  keys,
  CIO,
  filestr,
  screen,
  list,
  inputline,
  MIDFiles,
  nrpm,
  remote;
{$I-}

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
  refreshRate:Byte absolute $61;
  totalXMS:Byte absolute $62;

//
  memAvailable:longint;

// selector variables

  listScrAdr:array[0..15] of word absolute SCREEN_ADRSES;

  lstY:Byte absolute $60;
  lstOldY:Byte absolute $5a;
  lstShift:SmallInt absolute $5B;
  lstCurrent:SmallInt absolute $70;
  lstTotal:SmallInt absolute $72;

  curPlay:SmallInt absolute $74;

// counter

  timeShowMode:Byte = 3;

  counter:Longint; // absolute $76;
  cntBCD:Longint; // absolute $7a;
  seconds:Word;
  oLstY:byte;

//

{$i 'interrupt.inc'}
{$i 'osd.inc'}
{$i 's2_control.inc'}
{$i 'status.inc'}
{$i 'autostop_songchange.inc'}
{$i 'load.inc'}
{$i 'list.inc'}
procedure fileTypeByExt(s:PString); Forward;
{$i 'playlist.inc'}
{$i 'getdirectory.inc'}
{$i 'keyboard.inc'}
{$i 'init.inc'}

procedure registerKey; Assembler; Keep;
asm
  pha
  tya
  asl @
  tay
  pla
  sta MAIN.KEYS.KEY_TABLE_ADDR,y
  txa
  sta MAIN.KEYS.KEY_TABLE_ADDR+1,y
end;

procedure registerKeyboard; assembler;
asm
  .macro m@regKey key procVec
    ldy #MAIN.KEYS.:key
    lda #<MAIN.:procVec
    ldx #>MAIN.:procVec
    jsr registerKey
  .endm

  m@regKey k_L toggleLoopMode
  m@regKey k_I toggleMinMode
  m@regKey k_SPACE toggleFile
  m@regKey k_M toggleMeters
  m@regKey k_INVERS toggleScreenColors
  m@regKey k_T toggleTimeShowMode
  m@regKey k_HELP toggleHelpScreen
  m@regKey k_H toggleHelpScreen
  m@regKey k_RETURN fileAction
  m@regKey k_UP moveFileSelection
  m@regKey k_DOWN moveFileSelection
  m@regKey k_V playerCtrlMain
  m@regKey k_C playerCtrlMain
  m@regKey k_X playerCtrlMain
  m@regKey k_B playerCtrlSongChange
  m@regKey k_Z playerCtrlSongChange
  m@regKey k_COMMA ControlSetting
  m@regKey k_DOT ControlSetting
  m@regKey k_E ControlSetting
  m@regKey k_DELETE tempoControl
  m@regKey k_CLEAR tempoControl
  m@regKey k_INSERT tempoControl
  m@regKey k_ESC EXIT2dos
end;

begin
  asm
    lda PORTB
    pha
    lda #$FE
    sta PORTB
  end;
  init;
  registerKeyboard;

// '`outstr` is initialise by Loader and now its time to check for correct file specification
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

    // remoteControl;  // plugin

    if timer(otm,refreshRate) then
    begin
      resetTimer(otm);

      if isBitSet(screenStatus,ss_DLIchng) then
      begin
        clrBit(screenStatus,ss_DLIchng);

        if isBitClr(screenStatus,ss_minMode) then
          SDLST:=DLIST_ADDR
        else
          SDLST:=DLIST_MIN_ADDR;

        if isBitClr(screenStatus,ss_isHelp) then
        begin
          dpoke(DLIST_ADDR+24,SCREEN_WORK);
          lstY:=oLstY;
        end
        else
        begin
          dpoke(DLIST_ADDR+24,HELPSCR_ADDR);
          oLstY:=lstY;
          lstY:=255;
        end;
      end;

      if isBitSet(screenStatus,ss_isOSD) then
      begin
        if OSDTm>200 then OSDOff else inc(OSDTm,refreshRate);
      end;

      if isBitSet(screenStatus,ss_isRefresh) then
      begin
        clrBit(screenStatus,ss_isRefresh);
        showList;
      end;

      counter:=_timerTick;
      if isBitSet(playerStatus,ps_calcLen) and (_totalSongTicks>0) then
      begin
        _v:=(counter div _totalSongTicks)-1;
        if _v<>255 then
        asm
          icl 'asms/song_progress.a65'
        end;
      end;

      scradr:=screen_time+54;
      if timeShowMode=1 then
      begin
        putHex(counter,6);
      end else if timeShowMode=2 then
      begin
        asm icl 'asms/24bin_6bcd.a65' end;
        putHex(cntBCD,6);
      end else if timeShowMode=3 then
      begin
        calculateCurrentTime;
        seconds:=round(timeInSec);
        scradr:=screen_time+58;
        v:=seconds mod 60;
        asm icl 'asms/8bin_2bcd.a65' end;
        putHex(cntBCD,2);
        scradr:=screen_time+55;
        v:=seconds div 60;
        asm icl 'asms/8bin_2bcd.a65' end;
        putHex(cntBCD,2);
      end;

      asm icl 'asms/uvmeters_h.a65' end;

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

      if (stateInputLine and ils_mask_status)=ils_inprogress then do_inputLine;

      asm
        lda MAIN.KEYS.keyb
        bmi key_not_defined
        asl @
        tay

        lda MAIN.KEYS.KEY_TABLE_ADDR+1,y
        beq key_not_defined
        sta jump_addr+1
        lda MAIN.KEYS.KEY_TABLE_ADDR,y
        sta jump_addr

        jsr jump_addr:$ffff
      key_not_defined:
      end;

      keyb:=255;
    end;

  until false;

// End player loop
end.