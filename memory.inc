SCREEN_ADRSES   = $2380;              // $020 (  32)

CHARS_ADDR      = $2400;              // $400 (1024)

UVMETER_ADDR    = $2800;              // $040 (  64)

DLIST_ADDR      = UVMETER_ADDR+$40;   // $04C (  76)
DLIST_MIN_ADDR  = DLIST_ADDR+$4C;     // $037 (  55)

// -------------------------------------------------

SCREEN_ADDR     = $29C0;              // $730 (1840)


SCREEN_FOOT     = SCREEN_ADDR; // SCREEN_HEAD+HEAD_SIZE;
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

//

//SCREEN_HEAD     = SCREEN_STATUS+STATUS_SIZE;
HEAD_SIZE       = 19*40;              // $2f8 ( 760)

// -------------------------------------------------

HELPSCR_ADDR    = $3000;              // $2d0 ( 720)
SCREEN_HEAD     = HELPSCR_ADDR-HEAD_SIZE;

START_INFO_ADDR = $4000;              // $200 ( 512)
TRACK_DATA_ADDR = $3E00;              // $200 ( 512)

LIST_ADDR       = $4000;              //$1000 (4096)
