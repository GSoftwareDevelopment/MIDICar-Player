unit screen;

interface

var
  puttextinvert:Byte absolute $4F;
  DMACTL:Byte absolute $d400;
  SDLST:Word absolute $230;
  CHBASE:Byte absolute $2f4;
  scradr:Word absolute $58;

procedure waitFrame; Inline; Assembler;
procedure setColors; Assembler;
procedure invers(chars:byte); Register; Assembler;
procedure putSpaces(spaces:byte); Register; Assembler;
procedure putINTText(s:PString); Register; Assembler;
procedure PutASCText(s:PString); Register; Assembler;
procedure PutHex(var v; n:byte); Assembler;
procedure putInt(value:smallint); assembler;
procedure hline; assembler;
procedure clearWorkArea; assembler;

implementation

const
  Pow10Tab:Array[0..3] of word = (10000,1000,100,10);
  hexTab:Array[0..15] of char = '0123456789ABCDEF'~;

procedure waitFrame; Inline; Assembler;
asm
  lda $14
  cmp $14
  beq *-2
end;

procedure setColors; Assembler;
asm
  icl 'asms/screen/setColors.a65'
end;

procedure invers(chars:byte); Register; Assembler;
asm
  icl 'asms/screen/invers.a65'
end;

procedure putSpaces(spaces:byte); Register; Assembler;
asm
  icl 'asms/screen/put_spaces.a65'
end;

procedure putINTText(s:PString); Register; Assembler;
asm
  icl 'asms/screen/putinttext.a65'
end;

procedure PutASCText(s:PString); Register; Assembler;
asm
  icl 'asms/screen/putasctext.a65'
end;

procedure PutHex(var v; n:byte); Assembler;
asm
  icl 'asms/screen/puthex.a65'
end;

procedure putInt(value:smallint); assembler;
asm
  icl 'asms/screen/int2str.a65'
end;

procedure hline; assembler;
asm
  icl 'asms/screen/hline.a65'
end;

procedure clearWorkArea; assembler;
asm
  icl 'asms/screen/clear_workarea.a65'
end;

end.