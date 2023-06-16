unit keys;

interface

const
  k_L     = 0;
  k_J     = 1;
  k_SEMICO= 2;
  k_F1    = 3;
  k_F2    = 4;
  k_K     = 5;
  k_PLUS  = 6;
  k_LEFT  = 6;
  k_STAR  = 7;
  k_RIGHT = 7;
  k_O     = 8;
  // 9 - NOT USED
  k_P     = 10;
  k_U     = 11;
  k_RETURN= 12;
  k_I     = 13;
  k_UP    = 14;
  k_DOWN  = 15;
  k_V     = 16;
  k_HELP  = 17;
  k_C     = 18;
  k_F3    = 19;
  k_F4    = 20;
  k_B     = 21;
  k_X     = 22;
  k_Z     = 23;
  k_4     = 24;
  // 25 - NOT USED
  k_3     = 26;
  k_6     = 27;
  k_ESC   = 28;
  k_5     = 29;
  k_2     = 30;
  k_1     = 31;
  k_COMMA = 32;
  k_SPACE = 33;
  k_DOT   = 34;
  k_N     = 35;
  // 36 - NOT USED
  k_M     = 37;
  k_BACKSL= 38;
  k_INVERS= 39;
  k_R     = 40;
  // 41 - NOT USED
  k_E     = 42;
  k_Y     = 43;
  k_TAB   = 44;
  k_T     = 45;
  k_W     = 46;
  k_Q     = 47;
  k_9     = 48;
  // 49 - NOT USED
  k_0     = 50;
  k_7     = 51;
  k_DELETE= 52;
  k_8     = 53;
  k_CLEAR = 54;
  k_INSERT= 55;
  k_F     = 56;
  k_H     = 57;
  k_D     = 58;
  // 59 - NOT USED
  k_CAPS  = 60;
  k_G     = 61;
  k_S     = 62;
  k_A     = 63;

  kmod_shift = 64;
  kmod_control = 128;

var
  keyb:byte absolute $2fc;
  keymod:byte absolute $2fd;

procedure initKeyboard;
function keyscan2asc(keyscan:Byte):Byte; Assembler;
procedure doneKeyboard;

implementation

var
  VKEYBD:Pointer absolute $208;
  oldVKEYBD:Pointer;
  NMIEN:Byte absolute $D40E;

procedure keyConv; Assembler; Keep;
asm
  tay
  and #%11000000
  sta MAIN.KEYS.keymod
  tya
  and #%00111111
  sta MAIN.KEYS.keyb
end;

procedure keyboardInt; Interrupt; Assembler; Keep;
asm
  icl 'asms/interrupts/IRQ-keyboard.a65'
end;

procedure initKeyboard;
begin
  oldVKEYBD:=VKEYBD;
  NMIEN:=$00;

  VKEYBD:=@keyboardInt;
  NMIEN:=$40;
end;

function keyscan2asc(keyscan:Byte):Byte; Assembler;
asm
  icl 'asms/keyscan2asc.a65'
end;

procedure doneKeyboard;
begin
  NMIEN:=$00;
  VKEYBD:=oldVKEYBD;
  NMIEN:=$40;
end;

end.