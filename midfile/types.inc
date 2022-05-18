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
    status:byte;
    _event:Byte;
  end;

  TLoadingProcess = procedure(v:byte);
