procedure showHelp;
begin
  isHelp:=True;
  dpoke(DLIST_ADDR+25,HELPSCR_ADDR);
end;