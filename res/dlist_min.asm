    icl 'dlist.inc'
    icl '../memory.inc'
title_bar   = DL_MODE_320x192G2
work_area   = DL_MODE_40x24T2
vis_area    = DL_MODE_20X24T5
stat_area   = DL_MODE_20X12T5
prgs_area   = DL_MODE_160x96G4
foot_area   = DL_MODE_40x24T2

dl_start
    :2  dta DL_BLANK6
    :6  dta DL_BLANK8, DL_BLANK1

        dta title_bar + DL_LMS, A(SCREEN_HEAD)
    :18 dta title_bar

        dta DL_BLANK1

        dta work_area + DL_LMS, A(SCREEN_CHANNELS)
    :4  dta vis_area

        dta DL_BLANK1
        dta work_area + DL_LMS, A(SCREEN_TIME)
        dta DL_BLANK1
        dta stat_area

        dta prgs_area + DL_LMS, A(SCREEN_TIMELINE)
        dta DL_BLANK1
        dta work_area + DL_LMS, A(SCREEN_STATUS)

        dta DL_BLANK1
        dta foot_area + DL_LMS, A(SCREEN_FOOT)

        dta DL_JVB, A(dl_start)
