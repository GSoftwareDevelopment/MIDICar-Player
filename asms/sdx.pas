uses crt;

function DetectSDX:byte; assembler;
asm
    icl 'asms/detect_sdx'
    sta Result
end;

begin
    WriteLn(DetectSDX);
end.
