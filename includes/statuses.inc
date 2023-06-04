const
  statusMsgs : array[0..8] of String =
  (
    #$4e+'Playing'*~+#$ce,
    #$4e+'No song'*~+#$ce,
    #$4e+' Pause '*~+#$ce,
    #$4e+'Stopped'*~+#$ce,
    #$4e+'  Loading '*~,
    #$4e+'Calculating length'*~+#$ce,
    #$4e+'  Reading directory'*~+#$ce,
    #$4e+'  Reading description'*~+#$ce,
    #$4e+'  Reading playlist'*~+#$ce
  );

  errorMsgs : array[0..4] of String =
  (
    #$4e+'I/O Error #'*~,
    #$4e+'Unsupported format'~*+#$ce,
    #$4e+'Not enought memory'~*+#$ce,
    #$4e+'Not a MIDI file'~*+#$ce,
    #$4e+'List is full'~*+#$ce
  );