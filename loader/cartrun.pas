{$define ROMOFF}
uses Atari,zx5;

{$R cartrun.rc}

var dptr:pointer;

begin
  writeln('decompressing...');
  GetResourceHandle(dptr,'VARS');
  unZX5(dptr,pointer($0400));
  GetResourceHandle(dptr,'PROG');
  unZX5(dptr,pointer($8000));
  GetResourceHandle(dptr,'DATA');
  unZX5(dptr,pointer($2280));
  asm
    jmp $8857
  end;
end.