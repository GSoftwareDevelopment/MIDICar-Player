function ReadWordBI:Word;
begin
{$IFNDEF USE_CIO}
  blockRead(f,result{BI},2);
{$ELSE}
  BGet(CIO_CHN,@result,2);
{$ENDIF}
  asm
    ldx result
    ldy result+1
    stx result+1
    sty result
  end;
end;

function ReadLongBI:longint;
begin
{$IFNDEF USE_CIO}
  blockRead(f,result{BI},4);
{$ELSE}
  BGet(CIO_CHN,@result,4);
{$ENDIF}
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
