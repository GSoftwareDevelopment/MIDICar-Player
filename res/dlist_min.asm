    icl 'dlist.inc'
    icl '../memory.inc'

    opt h-
    org DLIST_MIN_ADDR

title_bar   = DL_MODE_320x192G2
work_area   = DL_MODE_40x24T2
vis_area    = DL_MODE_20X24T5
stat_area   = DL_MODE_20X12T5
prgs_area   = DL_MODE_160x96G4
foot_area   = DL_MODE_40x24T2

dl_start
    :2  dta DL_BLANK6
    :5  dta DL_BLANK8
        dta DL_BLANK8 + DL_DLI

        dta title_bar + DL_LMS, A(SCREEN_HEAD)
    :17 dta title_bar
        dta title_bar + DL_DLI

        dta DL_BLANK8
        dta vis_area + DL_LMS, A(SCREEN_CHANNELS+40)
    :3  dta vis_area

        dta DL_BLANK8
        dta stat_area + DL_LMS, A(SCREEN_TIME+40)

        dta prgs_area + DL_LMS, A(SCREEN_TIMELINE)
        dta DL_BLANK1 + DL_DLI
        dta work_area + DL_LMS, A(SCREEN_STATUS)

        dta DL_BLANK1 + DL_DLI
        dta foot_area + DL_LMS, A(SCREEN_FOOT)

        dta DL_JVB, A(dl_start)
