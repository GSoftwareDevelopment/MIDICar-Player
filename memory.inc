  SCREEN_ADDR     = $2800;              // $71c (1820)

  SCREEN_HEAD     = SCREEN_ADDR;        // $2f8 ( 760)
  SCREEN_FOOT     = SCREEN_HEAD+19*40;  // $0f0 ( 240)

  SCREEN_WORK     = SCREEN_FOOT+6*40;   // $2d0 ( 720)
  SCREEN_CHANNELS = SCREEN_WORK+12*40;
  SCREEN_TIME     = SCREEN_WORK+16*40;  // $014 (  20)
  SCREEN_STATUS   = SCREEN_TIME+20;     // $028 (  40)
  SCREEN_TIMELINE = SCREEN_STATUS+40;   // $028 (  40)

  CHARS_ADDR      = $3000;              // $400 (1024)

  KEY_TABLE_ADDR  = $3400;              // $080 ( 128)

  PROGRESS_DATA   = $3500;              // $004 (   4)
  UVMETER_ADDR    = PROGRESS_DATA+4;    // $040 (  64)
  SCREEN_ADRSES   = UVMETER_ADDR+$40;   // $020 (  32)
  HELPSCR_ADDR    = SCREEN_ADRSES+$20;  // $2d0 ( 720)
  DLIST_ADDR      = $2FAC;              // $054 (  84)

  LIST_ADDR       = $c000;              //$1000 (4096)
  START_INFO_ADDR = $3C00;              // $200 ( 512)
  TRACK_DATA_ADDR = $3E00;              // $200 ( 512)
