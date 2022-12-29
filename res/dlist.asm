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

        dta title_bar + DL_LMS, A(SCREEN_HEAD)
    :18 dta title_bar

        dta DL_BLANK1

        dta work_area + DL_LMS, A(SCREEN_WORK), DL_BLANK1
    :11 dta work_area, DL_BLANK1

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

    single_sum 'HEAD:' SCREEN_HEAD HEAD_SIZE
    single_sum 'FOOT:' SCREEN_FOOT FOOT_SIZE
    single_sum 'WORK:' SCREEN_WORK WORK_SIZE
    single_sum 'CHANNELS:' SCREEN_CHANNELS CHANNELS_SIZE
    single_sum 'TIME:' SCREEN_TIME TIME_SIZE
    single_sum 'TIMELINE:' SCREEN_TIMELINE TIMELINE_SIZE
    single_sum 'STATUS:' SCREEN_STATUS STATUS_SIZE
