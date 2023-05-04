procedure plugin_remoteControl;
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