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
procedure gotoNEntry(nEntry:SmallInt); assembler;
procedure addToList(entry:PString); Register; Assembler;

implementation

function getEntry(fn:PString):Boolean; register; assembler;
asm
  icl 'asms/list/getEntry.a65'
end;

procedure getEntrySets(var ptr); register; assembler;
asm
  icl 'asms/list/getEntrySets.a65'
end;

procedure setEntrySets(var ptr); register; assembler;
asm
  icl 'asms/list/setEntrySets.a65'
end;

function nextEntry:Boolean; assembler;
asm
  icl 'asms/list/nextEntry.a65'
end;

procedure gotoNEntry(nEntry:SmallInt); assembler;
asm
  icl 'asms/list/gotoNEntry.a65'
end;

procedure addToList(entry:PString); Register; Assembler;
asm
  icl 'asms/list/addToList.a65'
end;

end.