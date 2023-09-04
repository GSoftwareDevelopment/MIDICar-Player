{

}

type
  TTag = Longint;

  TDeltaVar = Longint;
  TByteArray = Array[0..0] of Byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    bank:Byte;              // 00
    ptr:Pointer;            // 00 00
    trackTime:TDeltaVar;    // 00 00 00 00
    _eStatusRun:Byte;       // 00
  end;

  TLoadingProcess = procedure(v:byte);
