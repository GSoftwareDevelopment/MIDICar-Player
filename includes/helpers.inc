var
  SOUNDR:Byte absolute $41;

procedure getCurrentPath(chn:byte; dev:PString; path:PString); register; assembler;
asm
  icl '../asms/getCurrentDirectory.a65'
end;

procedure resetMemoryPtr;
begin
  memAvailable:=(word(totalXMS)*16384)+FREE_MEM;
  last_bank:=totalXMS;
  if totalXMS>0 then
    last_adr:=$4000
  else
    last_adr:=$d800;
end;

procedure updateMem;
var
  oscradr:word; {?}
  memAKB:Word;  {?}

begin
  scradr:=SCREEN_FOOT; HLine;
  scradr:=SCREEN_FOOT; putINTText(VERSION);
  memAKB:=memAvailable div 1024;
  scradr:=SCREEN_FOOT+33;
  if memAKB<10000 then scradr:=SCREEN_FOOT+34;
  if memAKB<1000 then scradr:=SCREEN_FOOT+35;
  if memAKB<100 then scradr:=SCREEN_FOOT+36;
  if memAKB<10 then scradr:=SCREEN_FOOT+37;
  {putINTText(#$4a);} putInt(memAKB); putINTText('KB'~);
end;

procedure clearStatus;
var
  l:byte;

begin
  fillchar(pointer(SCREEN_TIME),100,0);
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$fd);
  statusLoop;
  updateMem;
  l:=length(fn);
  if l=0 then exit;
  if l>16 then fn[0]:=#16;
  scradr:=SCREEN_STATUS+40-l; putASCText(fn);
end;

procedure clearUVMeters; Assembler;
asm
  icl 'asms/uvmeters_clear.a65'
end;

procedure clearList;
begin
  resetMemoryPtr;
  fileList:=pointer(LIST_ADDR);
  totalFiles:=0; curfile:=0; YFile:=0; shFile:=0;
  chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    putSpaces(32);
    inc(chn);
  end;
  fillchar(fileList,$1000{$2700},$9B);
end;

procedure exit2DOS;
begin
  doneMIDI;
  Cls(0); Opn(0,12,0,'E:'); // SOUNDR:=1;
end;