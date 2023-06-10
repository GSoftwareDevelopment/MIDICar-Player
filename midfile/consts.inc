{

}

const
  f_counter = %10000000; // prevents counting
  f_tick    = %01000000; // tick indicator
  f_flags   = %11000000; // flags mask
  f_ratio   = %00001111; // timer divider mask

  f_skipDelta = %01000000;

  _trkRegs  = $e0;       // ZP registers for track processing

// MID file code formats
// __this player only supports 0 and 1 formats__
  MID_0 = 0;
  MID_1 = 1;

  ERR_UNSUPPORTED_FORMAT  = 100;
  ERR_NOT_ENOUGHT_MEMORY  = 101;
  ERR_NOT_A_MIDI_FILE     = 102;

  TAG_MTHD : TTag = $6468544D;
  TAG_MTRK : TTag = $6B72544D;

  sizeOf_TMIDTrack = sizeOf(TMIDTrack);

//
  TAB_OrC:Array[0..3] of byte = (
    %00000000,
    %01000000,
    %10000000,
    %11000000
  );

  TAB_OrD:Array[0..7] of byte = (
    %00000000,
    %00100000,
    %01000000,
    %01100000,
    %10000000,
    %10100000,
    %11000000,
    %11100000
  );