const
{$i memory.inc}
  FREE_MEM        = ($8000-$4000)+($d000-$c000)+($ff00-$d800);

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

  fl_device = $ff;
  fl_directory = $fe;
  fl_parentdir = $fd;
  fl_playlist = $fc;
  fl_midifile = $fb;
  fl_otherfile = $fa;

  // fe_PLS = $504c5300;
  fe_DIR = $c4c9d200;
  fe_MID = $4d494400;

  dev_size = 6;
  path_size = 64;
  name_size = 32;

  CURDEV_ADDR     = $4F0;
  CURPATH_ADDR    = $4F8;
  FN_PATH_ADDR    = $539;
  OUTSTR_ADDR     = $55A;
  SNULL_ADDR      = $5AB;

  dirMask = '*.*';

  colors:Array[0..1,0..4] of byte = (
    ($A8,$00,$0A,$08,$0A),
    ($A8,$0A,$00,$02,$00)
  );
