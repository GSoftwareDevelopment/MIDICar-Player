var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

procedure drawListSelection;
begin
  scradr:=playlistScrAdr[YFile]; invers(32);
end;

procedure showList;
var
  p_bank:byte;  //  \
  p_adr:word;   //  / It is important that this order remains intact

begin
  gotoNEntry(shFile); chn:=0;
  while (chn<16) do
  begin
    scradr:=playlistScrAdr[chn]; putSpaces(32);
    if getEntry(fn) then
    begin
      getEntrySets(@p_bank);
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

      if byte(fn[0])>LIST_VISIBLE_CHARS then fn[0]:=char(LIST_VISIBLE_CHARS);
      putASCText(fn);
    end
    else
      break;
    inc(chn);
    nextEntry;
  end;
end;

function changeDevice:Byte;
begin
  scradr:=playlistScrAdr[YFile];
  asm
    lda firstTime
    bne goFileSelector
  end;
  inputline(outstr);
  asm
    lda MAIN.INPUTLINE.Result
    bne goFileSelector
    jmp skip
  goFileSelector:
  end;

  fn:=outStr;
  if not getDeviceSpec(outstr,curDev) then joinStrings(curDev,fn);
  result:=fileSelect(outStr);
  asm
    skip:
  end;
  firstTime:=false;
end;

procedure choiceListFile;
var
  dev:TDevString;

begin
  curPlay:=255;
  statusStopped;
  gotoNEntry(curFile);
  if not getEntry(fn) then exit;
  getEntrySets(@p_bank);

  if not getDeviceSpec(fn,dev) then
    joinStrings(curDev,fn)
  else
    outstr:=fn;

//   if p_bank=$fd then IOResult:=loadPlaylist(outStr);
  if p_bank=$fe then IOResult:=changeDevice;
  if p_bank=$ff then
  begin
    _bank:=last_bank;
    _adr:=last_adr;
    curPlay:=curFile;
    IOResult:=loadSong(outStr);
  end;

  keyb:=255;
  clearStatus;
  showList;
  drawListSelection;
  if IOResult and %11111100<>0 then statusError(IOResult);
  if p_bank=$ff then statusPlaying;

  // if totalTracks<>0 then
  // begin
  //   last_bank:=_bank;
  //   last_adr:=adr;
  // end;
end;