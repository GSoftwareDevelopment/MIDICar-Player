function inputLine(var ln:String):Boolean;
var
  pos,ch:byte;
  oscradr:word;
  otm:byte;
  vcrs:boolean;

  procedure showLine;
  begin
    ln[0]:=char(pos);
    scradr:=oscradr;
    putSpaces(32); inc(scradr);
    putASCText(ln);
  end;

begin
  pos:=length(ln);
  fillchar(ln[pos+1],127-pos,$9b);
  keyb:=255; vcrs:=true;
  oscradr:=scradr; showLine; otm:=_tm;
  repeat
    if _tm-otm>10 then
    begin
      otm:=_tm;
      if vcrs then invers(1);
      vcrs:=not vcrs;
    end;
    if keyb=255 then continue;

    if keyb=k_ESC then exit(false);
    if pos>0 then
    begin
      if keyb=k_RETURN then exit(true);
      if (keyb=k_delete) then
      begin
        ln[pos]:=#$9B;
        dec(pos);
      end;
    end;
    if pos<30 then
    begin
      ch:=keyscan2asc(keyb);
      if ch<>0 then
      begin
        inc(pos);
        ln[pos]:=char(ch);
      end;
    end;
    showLine;
    keyb:=255;
  until false;
end;