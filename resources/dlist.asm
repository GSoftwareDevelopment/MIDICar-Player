    icl 'dlist.inc'

dl_start
    :2  dta DL_BLANK8

        dta DL_MODE_320x192G2 + DL_LMS, A(MAIN.SCREEN_ADDR);
    :18 dta DL_MODE_320x192G2

        dta DL_BLANK1

    :17 dta DL_MODE_40x24T2

        dta DL_MODE_20x12T5
        dta DL_BLANK1
        dta DL_MODE_40x24T2

        dta DL_BLANK1
    :20 dta DL_MODE_320x192G2

        dta DL_JVB, A(dl_start)