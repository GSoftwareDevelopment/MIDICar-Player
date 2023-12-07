unit remote;

interface

procedure sendClearPushLCD; Keep;
procedure sendPushLCDText(str:PString); register; Keep;
procedure remoteControl;
procedure updateSongTitle;

implementation
uses keys,list;

const
  ps_isStopped = %10000000;

var
  _v:byte absolute $D7;
  playerStatus:Byte absolute $D8;

procedure remoteControl;
const // System (Common/Real Time) Messages Table Convertion -> Keys
  SM2KEY:array[0..15] of byte = (
    255,        // F0 !!! Dont Touch !!!
    k_E,        // F1 Equaliser toggle
    k_L,        // F2 Loop mode toggle
    k_RETURN,   // F3 Return
    k_UP,       // F4 Up
    k_DOWN,     // F5 Down
    k_COMMA,    // F6 Volume Down
    255,        // F7 !!! Dont Touch !!!
    k_DOT,      // F8 Volume Up
    k_Z,        // F9 Prev
    k_X,        // FA Play/Pause
    255,        // FB n/a
    k_V,        // FC Stop
    k_B,        // FD Next
    255,        // FE n/a
    255         // FF !!! Dont Touch !!!
  );

begin
  asm
    jsr $200f
    bcc noDataReceive
    sta _v
    // sta SCREEN_FOOT
    and #$F0
    cmp #$F0
    bne noDataReceive
  end;

  if _v=$FA then // Play/Pause
  begin
    if playerStatus and ps_isStopped=0 then
      keyb:=k_C
    else
      keyb:=k_X;
  end
  else
    keyb:=SM2KEY[_v-$F0];

  asm
  noDataReceive:
  end;
end;

procedure sendClearPushLCD; Keep;
const
  push_sysex:array[0..5] of byte = (
    $f0, $47, $7f, $15, $00, $F7
  );

begin
  asm
  event = $ff

    // jsr $2003

    ldx #0

  sysex_header_loop:
    lda ADR.PUSH_SYSEX,x
    sta event
    jsr $2006
    inx
    cpx #6
    bne sysex_header_loop

    // sec       ; flush buffer & uninitialize driver
    // jsr $200c
  end;
end;

procedure sendPushLCDText(str:PString); Register; Keep;
const
  push_sysex:array[0..7] of byte = (
    $f0, $47, $7f, $15, $01, $00, $45, $00
  );

begin
  asm
  event = $ff

    lda str
    sta strSrc1
    sta strSrc2
    lda str+1
    sta strSrc1+1
    sta strSrc2+1

    // jsr $2003

    ldx #0
  sysex_header_loop:
    lda ADR.PUSH_SYSEX,x
    sta event
    jsr $2006
    inx
    cpx #8
    bne sysex_header_loop

  // sysex data loop
    ldx #1
  readLoop:
    lda strSrc2:$ffff,x
    jsr $2006

    inx
    cpx strSrc1:$ffff
    bmi readLoop
    beq readLoop

  // sysex end
    lda #$F7
    sta event
    jsr $2006

    // sec       ; flush buffer & uninitialize driver
    // jsr $200c
  end;
end;

procedure updateSongTitle;
const
  FN_PATH_ADDR = $539;
  SNULL_ADDR   = $5AB;

var
  Snull:String[80] absolute SNULL_ADDR;
  fn:String[32] absolute FN_PATH_ADDR;

begin
  asm
    jsr $2003 // start driver
  end;

  sendClearPushLCD;
  if IOResult<>ERR_LIST_ENTRY_END then
  begin
    list_getText(Snull);
    sendPushLCDText(Snull);
  end
  else
    sendPushLCDText(fn);
  asm
    sec \ jsr $200c // flush & stop driver
  end;
end;

end.