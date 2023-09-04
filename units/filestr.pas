unit filestr;

interface

const
  CURDEV_ADDR  = $4F0;
  CURPATH_ADDR = $4F8;
  FN_PATH_ADDR = $539;
  OUTSTR_ADDR  = $55A;
  SNULL_ADDR   = $5AB;

  dev_size  = 6;
  path_size = 64;
  name_size = 32;

  dp_dev  = 1;
  dp_path = 2;
  dp_name = 4;

type
  TDevString = String[dev_size];
  TPath      = String[path_size];
  TFilename  = String[name_size];

var
  curdev:TDevString absolute CURDEV_ADDR;   // 6 (+1) // absolute $4f0;
  curPath:TPath absolute CURPATH_ADDR;      // 64 (+1)
  fn:TFilename absolute FN_PATH_ADDR;       // 32 (+1)
  outstr:String[80] absolute OUTSTR_ADDR;   // 80 (+1)
  Snull:String[80] absolute SNULL_ADDR;     // 80 (+1)
  fnExt:DWord absolute $4dc;

procedure getFileExt(fn:PString); Register; assembler;
procedure reduceFileName(var inFN; outFN:PString); Register; Assembler;
function isDeviceSpec(fn:PString):Boolean; Register; Assembler;
function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler; Keep;
procedure joinStrings(s1:PString; s2:PString); Register; Assembler;
procedure getLn(chn:byte; buf:PString); register; Assembler;
function destructureFullPath(var inStr,devS,pathS,nameS:PString):byte; Register; Assembler;
procedure pathDelimiter(var inStr:PString; delimiterCh:Char); Register; Assembler;
procedure parentDir(var inStr:PString); Register; Assembler;
procedure getCurrentPath(chn:byte; dev:PString; path:PString); register; assembler;
procedure validPath;

implementation

procedure getFileExt(fn:PString); Register; assembler;
asm
  icl 'asms/filestr/get_file_ext.a65'
end;

procedure reduceFileName(var inFN; outFN:PString); Register; Assembler;
asm
  icl 'asms/filestr/reduce_filename.a65'
end;

function isDeviceSpec(fn:PString):Boolean; Register; Assembler;
asm
  icl 'asms/filestr/isDeviceSpec.a65'
end;

function getDeviceSpec(fn:PString; var spec:String):Boolean; Register; Assembler; Keep;
asm
  icl 'asms/filestr/getDeviceSpec.a65'
end;

procedure joinStrings(s1:PString; s2:PString); Register; Assembler;
asm
  icl 'asms/filestr/joinStrings.a65'
end;

procedure getLn(chn:byte; buf:PString); register; Assembler;
asm
  icl 'asms/filestr/get_line.a65'
end;

function destructureFullPath(var inStr,devS,pathS,nameS:PString):byte; Register; Assembler;
asm
  icl 'asms/filestr/destructureFullPath.a65'
end;

procedure pathDelimiter(var inStr:PString; delimiterCh:Char); Register; Assembler;
asm
  icl 'asms/filestr/path_delimiter.a65'
end;

procedure parentDir(var inStr:PString); Register; Assembler;
asm
  icl 'asms/filestr/parent_dir.a65'
end;

procedure getCurrentPath(chn:byte; dev:PString; path:PString); register; assembler;
asm
  icl '../asms/filestr/getCurrentDirectory.a65'
end;

procedure validPath;
var
  _v:byte absolute $D7;

begin
  _v:=destructureFullPath(outstr,curDev,curPath,fn);
  if (_v and dp_dev=0) then curDev:='D:';
  if (_v and dp_path=0) then getCurrentPath(2,curDev,curPath);
  PathDelimiter(curPath,'>');
end;

end.