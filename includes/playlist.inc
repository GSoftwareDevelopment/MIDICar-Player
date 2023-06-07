function readDescriptionFile(fname:PString):byte;
const
  CIO_CHN = 1;

var
  c0:byte absolute OUTSTR_ADDR;
  c1:char absolute OUTSTR_ADDR+1;

begin
  joinStrings(curDev,curPath);
  joinStrings(outStr,fname);
  opn(CIO_CHN,4,0,outStr);
  while IOResult=1 do
  begin
    getLn(CIO_CHN,outStr);
    if c0=0 then continue;
    if c1='*' then
    begin
      IOResult:=ERR_FILE_NOT_FOUND; break;
    end;
    v:=1; while (outStr[v]<>',') and (v<c0) do inc(v);
    if outStr[v]=',' then
    begin
      chn:=c0;
      if c1='>' then
      begin
        p_type:=fl_directory;
        dec(v);
        move(outStr[2],outStr[1],chn-1);
      end
      else
        p_type:=fl_midifile;
      c0:=v-1;
      list_putByte(p_type);
      list_putNText(outStr);

      chn:=chn-(v+1);
      move(outStr[v+1],outStr[1],chn);
      if chn>30 then
      begin
        chn:=30;
        outStr[30]:=#96;
      end;
      if outStr[chn]=#$9b then dec(chn);
      c0:=chn;
      list_putText(outStr);
      inc(lstTotal);
    end;
  end;
  p_type:=fl_otherfile;
  result:=IOResult;
  cls(CIO_CHN);
  clearStatus;
end;

function loadPlaylist:Byte; Register;
const
  CIO_CHN = 1;

begin
  scradr:=SCREEN_WORK; HLine;
  scradr:=SCREEN_WORK+29; putINTText(#$4e+'Playlist'~*+#$ce);

  statusStopped;
  validPath;
  clearList;

  putStatus(statusMsgs[8]);

  thi:=0;
  createListEntry(fl_device,outStr);

  result:=readDescriptionFile(fn);

  fn:='';
  clearStatus;
  thi:=255;
  screenStatus:=screenStatus or ss_isRefresh or ss_plsMode;
end;
