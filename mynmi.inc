var
  NMIEN:Byte absolute $D40E;
  NMIVEC:Pointer absolute $fffa;
  oldNMIVec:Pointer;

procedure MyNMI; Interrupt; Assembler;
asm
  icl 'asms/nmi.a65'
end;

procedure setNMI;
begin
  oldNMIVec:=NMIVEC;
  NMIEN:=$00; NMIVEC:=@MyNMI; NMIEN:=$40;
end;

procedure unsetNMI;
begin
  NMIEN:=$00; NMIVEC:=oldNMIVec; NMIEN:=$40;
end;