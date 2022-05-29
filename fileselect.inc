// procedure fileSelect;
// var
//   f:TSearchRec;
//   fileCount:Byte;

// begin
//   clearWorkarea;
//   FindFirst('D:*.MID',faAnyFile,f);
//   if IOResult=1 then
//   begin
//     while (IOResult=1) and (fileCount<32) do
//     begin
//     //   putText()
//       FindNext(f);
//     end;
//   end;
//   FindClose(f);
// end;
