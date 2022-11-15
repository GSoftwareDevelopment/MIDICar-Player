const
{$i memory.inc}

  VERSION= 'v1.5'~;
// base memory (under 64KB) available
  FREE_MEM = ($8000-$4000)+($ff00-$d800);

// input line status
  ils_pending    = 0;
  ils_inprogress = 1;
  ils_done       = 2;

// screen status
  ss_isRefresh = %10000000;  // flag for refresh list
  ss_isHelp    = %01000000;  // flag for help screen is shown
  ss_view      = %00100000;  // flag for volume meter is shown
  ss_colorSet  = %00010000;  // flag for color set mode

// player status
  ps_loop      = %1100;
  ps_playonce  = %0000;
  ps_repeatone = %0100;
  ps_repeatall = %1000;
  ps_shuffle   = %1100;
  ps_isStopped = %01000000;

//
  LIST_VISIBLE_CHARS = 30;
  LIST_VISIBLE_ITEMS = 16;

// file list flags
  fl_device    = $ff;
  fl_directory = $fe;
  fl_parentdir = $fd;
  fl_playlist  = $fc;
  fl_midifile  = $fb;
  fl_otherfile = $fa;

// file extentions
  // fe_PLS = $504c5300;
  fe_DIR = $c4c9d200;
  fe_MID = $4d494400;

//
  dev_size  = 6;
  path_size = 64;
  name_size = 32;

//
  CURDEV_ADDR  = $4F0;
  CURPATH_ADDR = $4F8;
  FN_PATH_ADDR = $539;
  OUTSTR_ADDR  = $55A;
  SNULL_ADDR   = $5AB;

// default search mask
  dirMask = '*.*';

// colors sets
  colors:Array[0..1,0..4] of byte = (
    ($A8,$00,$0A,$08,$0A),
    ($A8,$0A,$00,$02,$00)
  );
