const
  CHARS_ADDR      = $3000;
  SCREEN_ADDR     = $3400;
  SCREEN_WORK     = SCREEN_ADDR+19*40;
  SCREEN_TIME     = SCREEN_WORK+18*40;
  SCREEN_STATUS   = SCREEN_TIME+20;
  SCREEN_FOOT     = SCREEN_STATUS+40;
  UVMETER_ADDR    = $3B00;
  DLIST_ADDR      = $3FB7;

  PLAYLIST_ADDR   = $2000;
  START_INFO_ADDR = $2C00;
  TRACK_DATA_ADDR = $2E00;
  MIDI_DATA_ADDR  = $4000;
  FREE_MEM        = (($8000-$4000)+($d000-$a800)+($ff00-$d800)) div 1024;

  f_clear = %00100000;

  ps_colorSet = %001;
  ps_view     = %010;
  ps_loop     = %100;

  PLAYLIST_VISIBLE_CHARS = 30;
  fe_PLS = $504c5300;
  fe_DIR = $a4a9b200;
