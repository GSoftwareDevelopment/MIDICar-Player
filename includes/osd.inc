procedure OSDOff; Keep;
begin
  dpoke(DLIST_ADDR+58,SCREEN_TIME);
  dpoke(DLIST_MIN_ADDR+38,SCREEN_TIME+40);
  clrBit(screenStatus,ss_isOSD);
end;

procedure OSDOn; Keep;
begin
  dpoke(DLIST_ADDR+58,SCREEN_OSD);
  dpoke(DLIST_MIN_ADDR+38,SCREEN_OSD+40);
  setBit(screenStatus,ss_isOSD);
  OSDTm:=0;
end;
