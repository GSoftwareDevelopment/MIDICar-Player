function ReadWordBI:Word;
begin
  blockRead(f,result{BI},2);
  asm
    ldx result
    ldy result+1
    stx result+1
    sty result
  end;
end;

function ReadLongBI:longint;
begin
  blockRead(f,result{BI},4);
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
