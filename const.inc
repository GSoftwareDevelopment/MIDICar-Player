const
{$i memory.inc}

  VERSION= 'v1.5.8a'~;
// base memory (under 64KB) available
  FREE_MEM = ($8000-$4000)+($ff00-$d800);

// screen status
  ss_isRefresh = %10000000;  // flag for refresh list
  ss_isHelp    = %01000000;  // flag for help screen is shown
  ss_view      = %00100000;  // flag for volume meters is shown
  ss_colorSet  = %00010000;  // flag for color set mode
  ss_isOSD     = %00001000;  // flag for OSD is shown
  ss_minMode   = %00000100;  // flag for mini mode

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
  fl_device    = $ff;
  fl_directory = $fe;
  fl_parentdir = $fd;
  fl_playlist  = $fc;
  fl_midifile  = $fb;
  fl_otherfile = $fa;

// colors sets
  COLORS_ADDR = $4E0;

// MIDI Reset SysEx Message Address
  GM_RESET_ADDR = $3B00;

  ERR_FULL_LIST = 99;