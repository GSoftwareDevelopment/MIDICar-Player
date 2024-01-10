const
  controls:Array[0..3] of String[5] =
  (
    #$bb+#0+#$bc+#0+#$bd,   // all controls
    #$fb+#0+#0+#0+#0,       // play
    #0+#0+#$fc+#0+#0,       // pause
    #$0+#0+#$0+#0+#$fd      // stop
  );

  sloop:Array[0..3] of String[2] =
  (
    #$c0+#$c0,  // sloop_playone
    #$fe+#$ff,  // sloop_repeatone
    #$dc+#$dd,  // sloop_repeatall
    #$de+#$df   // sloop_shuffle
  );

  statusMsgs : array[0..7] of String =
  (
    #$4e+'Playing'*~+#$ce,
    #$4e+'No song'*~+#$ce,
    #$4e+' Pause '*~+#$ce,
    #$4e+'Stopped'*~+#$ce,
    #$4e+'  Loading '*~,
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