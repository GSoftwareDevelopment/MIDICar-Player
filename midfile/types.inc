{

}

type
  TTag = Longint;

  TDeltaVar = Longint;
  TByteArray = Array[0..0] of Byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    bank:Byte;
    ptr:Pointer;
    trackTime:TDeltaVar;
    _eStatusRun:Byte;
  end;

  TLoadingProcess = procedure(v:byte);
