{

}

type
  TTag = Longint;

  TDeltaVar = Longint;
  TByteArray = Array[0..0] of Byte;

  PMIDTrack = ^TMIDTrack;
  TMIDTrack = record
    status:byte;
    bank:Byte;
    ptr:Pointer;
    trackTime:TDeltaVar;
    _event:Byte;
  end;

  TLoadingProcess = procedure(v:byte);
