const
  SCREEN_ADRSES   = $0410;
  CHARS_ADDR      = $3000;
  SCREEN_ADDR     = $3400;
  SCREEN_WORK     = SCREEN_ADDR+19*40;
  SCREEN_TIME     = SCREEN_WORK+18*40;
  SCREEN_STATUS   = SCREEN_TIME+20;
  SCREEN_FOOT     = SCREEN_STATUS+40;
  UVMETER_ADDR    = $3B00;
  HELPSCR_ADDR    = UVMETER_ADDR+$40;
  DLIST_ADDR      = $3FB3;

  SCREEN_CHANNELS = SCREEN_WORK+42;

  PLAYLIST_ADDR   = $2000;
  START_INFO_ADDR = $2C00;
  TRACK_DATA_ADDR = $2E00;
  MIDI_DATA_ADDR  = $4000;
  FREE_MEM        = (($8000-$4000)+($d000-$a800)+($ff00-$d800)) div 1024;

  f_clear = %00100000;

  ps_colorSet = %0001;
  ps_view     = %0010;
  ps_loop     = %1100;

  ps_playonce = %0000;
  ps_repeatone= %0100;
  ps_repeatall= %1000;
  ps_shuffle  = %1100;

  ps_isRefresh = %10000000;
  ps_isStopped = %01000000;
  ps_isHelp    = %00100000;

  LIST_VISIBLE_CHARS = 30;
  LIST_VISIBLE_ITEMS = 16;
  
  // fe_PLS = $504c5300;
  fe_DIR = $a4a9b200;
  fe_MID = $4d494400;