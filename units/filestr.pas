unit filestr;

interface

procedure getFileExt(fn:PString); Register; assembler;
procedure reduceFileName(var inFN; outFN:PString); Register; Assembler;
function isDeviceSpec(fn:PString):Boolean; Register; Assembler;
function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler;
procedure joinStrings(s1:PString; s2:PString); Register; Assembler;
procedure getLn(chn:byte; buf:PString); register; Assembler;

implementation

procedure getFileExt(fn:PString); Register; assembler;
asm
  icl 'asms/get_file_ext.a65'
end;

procedure reduceFileName(var inFN; outFN:PString); Register; Assembler;
asm
  icl 'asms/reduce_filename.a65'
end;

function isDeviceSpec(fn:PString):Boolean; Register; Assembler;
asm
  icl 'asms/isDeviceSpec.a65'
end;

function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler;
asm
  icl 'asms/getDeviceSpec.a65'
end;

procedure joinStrings(s1:PString; s2:PString); Register; Assembler;
asm
  icl 'asms/joinStrings.a65'
end;

procedure getLn(chn:byte; buf:PString); register; Assembler;
asm
  icl 'asms/get_line.a65'
end;

end.