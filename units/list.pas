unit list;

interface

var
  fileList:Pointer absolute $DA;
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

function getEntry(fn:PString):Boolean; register; assembler;
procedure getEntrySets(var ptr); register; assembler;
procedure setEntrySets(var ptr); register; assembler;
function nextEntry:Boolean; assembler;
procedure gotoNEntry(nEntry:byte); assembler;
procedure addToList(entry:PString); Register; Assembler;

implementation

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

end.