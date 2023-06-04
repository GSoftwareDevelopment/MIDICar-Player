unit list;

interface

const
  ERR_LIST_FOUND     = 96;
  ERR_LIST_ENTRY_END = 97;
  ERR_LIST_END       = 98;
  ERR_LIST_IS_FULL   = 99;

var
  listPtr:Pointer absolute $DA;
  p_type:byte;

{
Fetch string to `fn` from current pointer `listPtr`.

After this operation, check IOResult!
The lack of change in IOResult, shows that the end of the record has not been reached!
}
procedure list_getText(fn:PString); register; assembler;

{
Adds a new entry to the list at the location indicated by the `listPtr` pointer.

list_putText  writes a string ending in an inverted
              it's also an indication of the end of the record!
list_putNText writes a null-terminated string
              this does not end the record.

The pointer is incremented by the length of the string (+1 when null-terminated).
Returns an error when it reaches the list limit.
}
procedure list_putText(entry:PString); Register; Assembler;
procedure list_putNText(entry:PString); Register; Assembler;

{
It retrieves one byte from the location that `listPtr` points to.

If the fetched byte is set to the 7th bit, it will be treated as the
end of the record (ERR_LIST_ENTRY_END error).
When it is equal to $9B (Atari EOL) an ERR_LIST_END error will be returned.
}
procedure list_getByte(var ptr); register; assembler;

{
Put one byte in current location that `listPtr` points to.

The pointer is incremented by 1 byte.
Returns an error when it reaches the list limit.
}
procedure list_putByte(var ptr); register; assembler;

{
Moves the `listPtr` pointer to the next record.

The operation can be performed when the end of the record has not been reached.
}
procedure list_nextEntry; assembler; keep;

{
Sets the `listPtr` pointer to the nth element of the list.
}
procedure list_go2Entry(nEntry:SmallInt); assembler;

implementation

procedure list_getText(fn:PString); register; assembler;
asm
  icl 'asms/list/getText.a65'
end;

procedure list_putText(entry:PString); Register; Assembler;
asm
  icl 'asms/list/putText.a65'
end;

procedure list_putNText(entry:PString); Register; Assembler;
asm
  icl 'asms/list/putNText.a65'
end;

procedure list_getByte(var ptr); register; assembler;
asm
  icl 'asms/list/getSets.a65'
end;

procedure list_putByte(var ptr); register; assembler;
asm
  icl 'asms/list/putSets.a65'
end;

procedure list_nextEntry; assembler; keep;
asm
  icl 'asms/list/nextEntry.a65'
end;

procedure list_go2Entry(nEntry:SmallInt); assembler;
asm
  icl 'asms/list/gotoEntry.a65'
end;

end.