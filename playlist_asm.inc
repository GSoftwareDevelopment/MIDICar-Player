function getEntry(fn:PString):Boolean; register; assembler;
asm
  icl 'asms/playlist_getEntry.a65'
end;

procedure getEntrySets(var ptr); register; assembler;
asm
  icl 'asms/playlist_getEntrySets.a65'
end;

function nextEntry:Boolean; assembler;
asm
  icl 'asms/playlist_nextEntry.a65'
end;

procedure gotoNEntry(nEntry:byte); assembler;
asm
  icl 'asms/playlist_gotoNEntry.a65'
end;

procedure addToPlaylist(entry:PString); Register; Assembler;
asm
  icl 'asms/playlist_addToPlaylist.a65'
end;