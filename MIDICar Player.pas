Uses MC6850,MIDI_FIFO,MIDFiles;

Procedure MIDI_SendNoteOn(ch,note,vel:byte);
begin
  ZP_Data:=%10010000+ch and $0f;
  FIFO_WriteByte(ZP_Data);
  ZP_Data:=note and $7f;
  FIFO_WriteByte(ZP_Data);
  ZP_Data:=vel and $7f;
  FIFO_WriteByte(ZP_Data);
end;

begin
  FIFO_Reset;
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits);

  MIDData:=Pointer($6000);
  MIDTracks:=Pointer($5F00);
  LoadMID('D2:OVERWORL.MID');

//  MIDI_SendNoteOn(0,64,64);
//  FIFO_Flush;
end.