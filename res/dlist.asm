    icl 'dlist.inc'
    icl '../memory.inc'

dl_start
    :2  dta DL_BLANK6

        dta DL_MODE_320x192G2 + DL_LMS, A(SCREEN_HEAD)
    :18 dta DL_MODE_320x192G2

        dta DL_BLANK1

        dta DL_MODE_40x24T2 + DL_LMS, A(SCREEN_WORK), DL_BLANK1
    :17 dta DL_MODE_40x24T2, DL_BLANK1

        dta DL_MODE_20x12T5 + DL_LMS, A(SCREEN_TIME)
        dta DL_MODE_160x96G4 + DL_LMS, A(SCREEN_TIMELINE)
        dta DL_BLANK1
        dta DL_MODE_40x24T2 + DL_LMS, A(SCREEN_TIME + 20)

        dta DL_BLANK1
        dta DL_MODE_40x24T2 + DL_LMS, A(SCREEN_FOOT)

        dta DL_JVB, A(dl_start)