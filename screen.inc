const
  colors:Array[0..1,0..4] of byte = (
    ($A8,$00,$0A,$08,$0A),
    ($A8,$0A,$00,$02,$00)
  );

var
  puttextinvert:byte;
  SDMACTL:Byte;
  DMACTL:Byte absolute $d400;
  SDLST:Word absolute $230;
  CHBASE:Byte absolute $2f4;

procedure setColors; assembler;
asm
  icl 'asms/setColors.a65'
end;

procedure invers(chars:byte); Register; Assembler;
asm
  icl 'asms/invers.a65'
end;

procedure putSpaces(spaces:byte); Register; Assembler;
asm
  icl 'asms/put_spaces.a65'
end;

procedure putINTText(s:PString); Register; Assembler;
asm
  icl 'asms/putinttext.a65'
end;

procedure PutASCText(s:PString); Register; Assembler;
asm
  icl 'asms/putasctext.a65'
end;

procedure PutHex(var v; n:byte); Assembler;
asm
  icl 'asms/puthex.a65'
end;

procedure putInt(value:smallint); assembler;
asm
  icl 'asms/int2str.a65'
end;

procedure hline; assembler;
asm
  icl 'asms/hline.a65'
end;

procedure clearWorkArea; assembler;
asm
  icl 'asms/clear_workarea.a65'
end;
