const
  k_l     = 0;
  k_left  = 6;
  k_right = 7;
  k_p     = 10;
  k_return= 12;
  k_up    = 14;
  k_down  = 15;
  k_esc   = 28;
  k_space = 33;
  k_m     = 37;
  k_invers= 39;
  k_tab   = 44;
  k_delete= 52;
  k_clear = 54;
  k_insert= 55;
  k_caps  = 60;
  k_s     = 62;

function keyscan2asc(keyscan:Byte):Byte; Assembler;
asm
  icl 'asms/keyscan2asc.a65'
end;
