uses crt;

procedure Detect_MC6850; Assembler; Keep;
asm
  icl 'asm/detect_mc.a65'
end;

begin
  Detect_MC6850;
  asm
    sty $d8
    stx $d9
    bcc MCFound
  end;

  WriteLn('MIDICar not found :(');

  asm
    jmp ($a)
  end;

  asm
    MCFound:
  end;
end.