var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

procedure drawListSelection;
begin
  scradr:=listScrAdr[YFile]; invers(32);
end;

procedure showEntry;
var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

begin
  getEntrySets(p_bank);
  v:=0;
//   if p_bank=$FD then v:=$5b; // playlist file
  if p_bank=$FE then v:=$1e; // current path
  if p_bank=$FF then v:=$5a; // song
  if p_bank<$40 then
    if p_adr=$00 then
      v:=$4d  // song not loaded
    else
      v:=$5a; // file loaded into memory

  poke(scradr,v); inc(scradr,2);
  v:=byte(outstr[0]);
  if v>LIST_VISIBLE_CHARS then outstr[0]:=char(LIST_VISIBLE_CHARS);
  putASCText(outstr);
  putSpaces(30-v);
end;

procedure showList;
begin
  gotoNEntry(shFile); chn:=0;
  while (chn<LIST_VISIBLE_ITEMS) do
  begin
    scradr:=listScrAdr[chn];
    if not getEntry(outstr) then break;
    showEntry;
    inc(chn);
    nextEntry;
  end;
end;

function choiceListFile:boolean;
var
  dev:TDevString;

begin
  if stateInputline<>0 then exit(false);

  gotoNEntry(curFile);
  result:=getEntry(fn);
  if result then
  begin
    getEntrySets(p_bank);

    if not getDeviceSpec(fn,dev) then
      joinStrings(curDev,fn)
    else
      outstr:=fn;
  end;
end;
