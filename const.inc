const
{$i memory.inc}

  VERSION= 'dev'~;
// base memory (under 64KB) available
  BASEMEM_FREE = 0; // ($8000-$4000)+($ff00-$d800);

// screen status
  ss_isRefresh = %10000000;  // flag for refresh list
  ss_isHelp    = %01000000;  // flag for help screen is shown
  ss_view      = %00100000;  // flag for volume meters is shown
  ss_isOSD     = %00010000;  // flag for OSD is shown
  ss_colorSet  = %00001000;  // flag for color set mode
  ss_minMode   = %00000100;  // flag for mini mode
  ss_plsMode   = %00000010;  // flag for playlist mode

// player status
  ps_loop      = %00000011;
  ps_playonce  = %00000000;
  ps_repeatone = %00000001;
  ps_repeatall = %00000010;
  ps_shuffle   = %00000011;
  ps_isStopped = %10000000;
  ps_calcLen   = %01000000;

//
  LIST_VISIBLE_CHARS = 30;
  LIST_VISIBLE_ITEMS = 11;

// file list flags
  fl_device    = $7f; // $ff;
  fl_directory = $7e; // $fe;
  fl_parentdir = $7d; // $fd;
  fl_playlist  = $7c; // $fc;
  fl_curPlay   = $7b;
  fl_midifile  = $7a; // $fb;
  fl_otherfile = $79; // $fa;

// file extentions
  fe_PLS = $504c5300;
  fe_DIR = $c4c9d200;
  fe_MID = $4d494400;
  fe_ION = $494F4E00;

// default search mask
  dirMask = '*.*';
  descriptionFile = 'DESCRIPT.ION';

// colors sets
  COLORS_ADDR = $4E0;

// MIDI Reset SysEx Message Address
  GM_RESET_ADDR = $3B00;

// errors
  ERR_FILE_NOT_FOUND = 170;

TAB_PROGRESS_DATA:Array[0..3] of byte = (
  %01000000,
  %01010000,
  %01010100,
  %01010101
);
