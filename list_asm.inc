function getEntry(fn:PString):Boolean; register; assembler;
asm
  icl 'asms/list_getEntry.a65'
end;

procedure getEntrySets(var ptr); register; assembler;
asm
  icl 'asms/list_getEntrySets.a65'
end;

procedure setEntrySets(var ptr); register; assembler;
asm
  icl 'asms/list_setEntrySets.a65'
end;

function nextEntry:Boolean; assembler;
asm
  icl 'asms/list_nextEntry.a65'
end;

procedure gotoNEntry(nEntry:byte); assembler;
asm
  icl 'asms/list_gotoNEntry.a65'
end;

procedure addToList(entry:PString); Register; Assembler;
asm
  icl 'asms/list_addToList.a65'
end;

procedure clearList;
begin
  resetMemoryPtr;
  fileList:=pointer(PLAYLIST_ADDR);
  totalFiles:=0; curfile:=0; YFile:=0; shFile:=0;
  chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    putSpaces(32);
    inc(chn);
  end;
  fillchar(fileList,$C00,$9B);
end;
