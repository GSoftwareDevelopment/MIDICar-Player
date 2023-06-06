scan2asc:        // from 32 to 95
    .byte 33   //   space
    .byte 95   // ! exclamation mark
    .byte 255  // " quote mark
    .byte 90   // # hash
    .byte 255  // $ dolar
    .byte 93   // % percent
    .byte 255  // & and
    .byte 255  // '
    .byte 255  // (
    .byte 255  // )
    .byte 7    // * star
    .byte 255  // + plus
    .byte 255  //  comma
    .byte 14   // - hypen
    .byte 34   // . dot
    .byte 38   // / slash
    .byte 50,31,30,26,24,29,27,51,53,48 // 0-9 digits
    .byte 66   // :colon
    .byte 255  // ; semicolon
    .byte 54   // < less sign
    .byte 255  // = equal
    .byte 55   // > more sign
    .byte 102  // ? question mark
    .byte 255  // @ at
    .byte 63,21,18,58,42,56,61,57,13,1,5,0,37,35,8,10,47,40,62,45,11,16,46,22,43,23 // A-Z letters
    .byte 255  // [
    .byte 70   // \ backslash
    .byte 255  // ]
    .byte 255  // ^
    .byte 78    // _ underscore mark
