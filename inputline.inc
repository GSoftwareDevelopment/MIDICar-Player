
procedure show_inputLine;
begin
  outstr[0]:=char(ilpos);
  scradr:=ilscradr;
  putASCText(outstr);
  if ilvcrs then ilch:=$80 else ilch:=$00;
  poke(scradr,ilch);
  inc(scradr); putSpaces(31-ilpos);
end;

procedure init_inputLine;
begin
  ilpos:=length(outstr);
  fillchar(outstr[ilpos+1],127-ilpos,$9b);
  keyb:=255; ilvcrs:=true;
  ilscradr:=listScrAdr[YFile]+2;
  show_inputLine; ctm:=_tm;
  stateInputLine:=1;
  resultInputLine:=false;
end;

procedure do_inputLine;
begin
  if ilpos>0 then
  begin
    if (keyb=k_ESC) or (keyb=k_RETURN) then
    begin
      if keyb=k_ESC then
      begin
        ilpos:=byte(fn[0]);
        outstr:=fn;
      end;
      ilvcrs:=false; show_inputLine;
      stateInputLine:=2;
      resultInputLine:=(keyb=K_RETURN);
      keyb:=k_RETURN; exit;
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
  keyb:=255;
end;