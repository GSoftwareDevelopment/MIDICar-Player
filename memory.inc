SCREEN_ADDR     = $2800;              // $604 (1540)
DLIST_ADDR      = $2FB3;              // $04C (  76)

  SCREEN_HEAD     = SCREEN_ADDR;        // $2f8 ( 760)
  SCREEN_FOOT     = SCREEN_HEAD+19*40;  // $028 (  40)
  SCREEN_WORK     = SCREEN_FOOT+40;     // $2d0 ( 720)
  SCREEN_CHANNELS = SCREEN_WORK+12*40;  // $0a0 ( 160)
  SCREEN_TIME     = SCREEN_WORK+16*40;  // $014 (  20)
  SCREEN_STATUS   = SCREEN_TIME+20;     // $028 (  40)
  SCREEN_TIMELINE = SCREEN_STATUS+40;   // $028 (  40)

CHARS_ADDR      = $3000;              // $400 (1024)
KEY_TABLE_ADDR  = $3400;              // $080 ( 128)
UVMETER_ADDR    = $3500;              // $040 (  64)
  SCREEN_ADRSES   = UVMETER_ADDR+$40;   // $020 (  32)
  HELPSCR_ADDR    = SCREEN_ADRSES+$20;  // $2d0 ( 720)

START_INFO_ADDR = $3C00;              // $200 ( 512)
TRACK_DATA_ADDR = $3E00;              // $200 ( 512)

LIST_ADDR       = $c000;              //$1000 (4096)
