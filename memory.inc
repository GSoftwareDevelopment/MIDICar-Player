CHARS_ADDR      = $2400;              // $400 (1024)
SCREEN_ADDR     = $2800;              // $6A4 (1700)

UVMETER_ADDR    = $2EA0;              // $040 (  64)
SCREEN_ADRSES   = UVMETER_ADDR+$40;   // $020 (  32)
KEY_TABLE_ADDR  = $2380;              // $080 ( 128)

TAB_SCAN2ASC    = $2F00;              // $040 (  64)
TAB_NRPM_ADDR   = $2F40;              // $036 (  54)
DLIST_MIN_ADDR  = $2F7C;
DLIST_ADDR      = $2FB4;              // $04C (  76)

// -------------------------------------------------

SCREEN_HEAD     = SCREEN_ADDR;
HEAD_SIZE       = 19*40;              // $2f8 ( 760)

SCREEN_FOOT     = SCREEN_HEAD+HEAD_SIZE;
FOOT_SIZE       = 40;                 // $028 (  40)

SCREEN_WORK     = SCREEN_FOOT+FOOT_SIZE;
WORK_SIZE       = 12*40;              // $2d0 ( 720)

SCREEN_CHANNELS = SCREEN_WORK+WORK_SIZE;
CHANNELS_SIZE   = 40+4*20;            // $078 ( 120)

SCREEN_TIME     = SCREEN_CHANNELS+CHANNELS_SIZE;
TIME_SIZE       = 40+20;              // $03C (  60)

SCREEN_TIMELINE = SCREEN_TIME+TIME_SIZE;
TIMELINE_SIZE   = 40;                 // $028 (  40)

SCREEN_OSD      = SCREEN_TIMELINE+TIMELINE_SIZE;
OSD_SIZE        = 40+20;              // $03C (  60)

SCREEN_STATUS   = SCREEN_OSD+OSD_SIZE;
STATUS_SIZE     = 40;                 // $028 (  40)

// -------------------------------------------------

HELPSCR_ADDR    = $3000;              // $2d0 ( 720)
START_INFO_ADDR = $3C00;              // $200 ( 512)
TRACK_DATA_ADDR = $3E00;              // $200 ( 512)

LIST_ADDR       = $4000;              //$1000 (4096)
