{$LIBRARYPATH midfile/}
{$DEFINE ROMOFF}
Uses
  MC6850,
  MIDI_FIFO,
  MIDFiles,
  sysutils,
  Misc,
  CIO;
{$I-}

const
  DLIST_ADDR      = $3400;
  SCREEN_ADDR     = $3430;
  SCREEN_STATUS   = SCREEN_ADDR+25*40;
  TRACK_DATA_ADDR = $3E00;
  MIDI_DATA_ADDR  = $4000;
  FREE_MEM        = (($8000-$4000)+($d000-$a000)+($ff00-$e000)) div 1024;

{$r selftest.rc}

var
  chptr:array[0..39] of word;
  scradr:Word;

// {$I mc6850_irq.inc}
{$i helpers.inc}

procedure clearStatus;
begin
  fillchar(pointer(SCREEN_STATUS),40,128);
end;

procedure init;
var
  totalXMS:word;
  x,y:byte;

begin
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}
  // copy charset definition
  move(pointer($e000),pointer($3000),$400);
  poke(709,15); poke(710,0);
  poke($2F4,$30);
  fillchar(pointer(SCREEN_ADDR),1040,0);
  dpoke($230,DLIST_ADDR);

  fillchar(pointer(SCREEN_ADDR),40,128);
  scradr:=SCREEN_ADDR+1; PutText('MIDICar Player RC1'~*);
  scradr:=SCREEN_ADDR+31; PutText('2022 GSD'~*);
  clearStatus;

  // detect memory
  totalXMS:=DetectMem*16;
  asm lda #$fe \ sta $100 end;
  // writeLn('Available memory: ',FREE_MEM+totalXMS,' KB');

  MIDTracks:=Pointer(TRACK_DATA_ADDR);
  MIDData:=Pointer(MIDI_DATA_ADDR);

  for cTrk:=0 to 39 do
  begin
    x:=cTrk div 20; y:=cTrk mod 20;
    chPtr[cTrk]:=SCREEN_ADDR+80+80+x*20+y*40;
  end;
end;

procedure loadPrc(v:byte);
var
  i:byte;

begin
  scradr:=SCREEN_STATUS+10;
  putInt(v-1); poke(scradr,$8f); inc(scradr);
  putInt(totalTracks);
end;

procedure checkParams;
var
  fn:PString;
  err:shortint;

begin
  if paramCount=1 then
  begin
    fn:=ParamStr(1);
    scradr:=SCREEN_STATUS+1; PutText('Loading ../..'~*);
    puttextinvert:=128;
    loadProcess:=@loadPrc;
    err:=LoadMid(fn);
    if err<>0 then
    begin
      scradr:=SCREEN_STATUS+1;
      if err>0 then
      case err of
        ERR_UNSUPPORTED_FORMAT: PutText('Unsupported format'~*);
        ERR_NOT_ENOUGHT_MEMORY: PutText('Not enought memory'~*);
      end
      else
      begin
        PutText('I/O Error #'~*); putInt(err);
      end;
      while keyb=255 do ;
      keyb:=255;
      exit2DOS;
    end;
  end
  else
  begin
    totalTracks:=1;
  end;
end;

begin
  init;
  checkParams;

//
  clearStatus;
  scradr:=SCREEN_STATUS+1; PutText('Playing'~*);

  // initialize MIDICar (MC6850)
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+IRQ_Receive);

  // initialize IRQ for MC6850
  // oldIRQ:=VIMIRQ; asm SEI end; VIMIRQ:=@MC6850_IRQ; asm CLI end;

  Reset_MIDI;        // reset MIDI
  _totalTicks:=0;    // reset song ticks
  setTempo;          // set song tempo

// Player loop

  cTrk:=totalTracks;
  Repeat

    processMIDI;

    // scradr:=chptr[cTrk];
    // PutByteHex(_bank); inc(scradr,3);
    // PutWordHex(_adr); inc(scradr,6);
    // PutLongHex(_trackTime); // _delta

{$IFDEF USE_FIFO}
    FIFO_PushDirect2MC6850;
{$ENDIF}

    if keyb<>255 then
    begin
      case keyb of
        k_esc: break;
        k_p:
          begin
            _timerStatus:=_timerStatus xor f_counter;
            scradr:=SCREEN_STATUS+1;
            if _timerStatus and f_counter=0 then
              PutText('Playing'~*)
            else
              PutText('Pause  '~*);
          end;
      end;
      keyb:=255;
    end;
  until (PlayingTracks=0);

  // Write(#156);

// End player loop

  // poke(559,34);
  // dpoke($9c24,$9c40);

  Reset_MIDI;

  // restore IRQ vector
  // asm SEI end; VIMIRQ:=oldIRQ; asm CLI end;

  // resotre Timer vector
  setIntVec(iTim1,oldTimerVec);
  exit2DOS;
end.