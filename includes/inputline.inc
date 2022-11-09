
procedure show_inputLine;
begin
  outstr[0]:=char(ilpos);
  scradr:=ilscradr;
  putASCText(outstr);
  if stateInputLine=ils_inprogress then
  begin
    asm
      ldy #0
      lda (scradr),y
      bmi nospace
      lda #0
    noSpace:
      eor #$80
      sta (scradr),y
    end;
    inc(scradr);
  end;
  putSpaces(31-ilpos);
end;

procedure init_inputLine;
begin
  ilpos:=length(outstr);
  fillchar(outstr[ilpos+1],80-ilpos,$9b);
  keyb:=255;
  show_inputLine; ctm:=_tm;
  stateInputLine:=ils_inprogress;
  resultInputLine:=false;
end;

procedure do_inputLine;
begin
  if ilpos>0 then
  begin
    if (keyb=k_ESC) or (keyb=k_RETURN) then
    begin
      if keyb=k_ESC then outStr:=Snull;
      ilpos:=byte(outstr[0]);
      stateInputLine:=ils_done;
      show_inputLine;
      resultInputLine:=(keyb=K_RETURN);
      // keyb:=k_RETURN;
      exit;
    end;
    if (keyb=k_delete) then
    begin
      outstr[ilpos]:=#$9B;
      dec(ilpos);
    end;
  end;
  if ilpos<30 then
  begin
    ilch:=keyscan2asc(keyb);
    if ilch<>0 then
    begin
      inc(ilpos);
      outstr[ilpos]:=char(ilch);
    end;
  end;
  show_inputLine;
  // keyb:=255;
end;