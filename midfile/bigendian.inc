function ReadWordBI:Word;
begin
  BGet(CIO_CHN,@result,2);
  asm
    ldx result
    ldy result+1
    stx result+1
    sty result
  end;
end;

function ReadLongBI:longint;
begin
  BGet(CIO_CHN,@result,4);
  asm
    ldx result
    ldy result+3
    stx result+3
    sty result

    ldx result+1
    ldy result+2
    stx result+2
    sty result+1
  end;
end;
