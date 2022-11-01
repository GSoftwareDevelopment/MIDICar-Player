unit filestr;

interface

const
  dp_dev  = 1;
  dp_path = 2;
  dp_name = 4;

procedure getFileExt(fn:PString); Register; assembler;
procedure reduceFileName(var inFN; outFN:PString); Register; Assembler;
function isDeviceSpec(fn:PString):Boolean; Register; Assembler;
function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler; Keep;
procedure joinStrings(s1:PString; s2:PString); Register; Assembler;
procedure getLn(chn:byte; buf:PString); register; Assembler;
function destructureFullPath(var inStr,devS,pathS,nameS:PString):byte; Register; Assembler;
procedure pathDelimiter(var inStr:PString; delimiterCh:Char); Register; Assembler;
procedure parentDir(var inStr:PString); Register; Assembler;

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

function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler; Keep;
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

function destructureFullPath(var inStr,devS,pathS,nameS:PString):byte; Register; Assembler;
asm
  icl 'asms/destructureFullPath.a65'
end;

procedure pathDelimiter(var inStr:PString; delimiterCh:Char); Register; Assembler;
asm
  icl 'asms/path_delimiter.a65'
end;

procedure parentDir(var inStr:PString); Register; Assembler;
asm
  icl 'asms/parent_dir.a65'
end;

end.