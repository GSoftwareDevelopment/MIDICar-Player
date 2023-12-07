unit NRPM;

interface
const
  TAB_NRPM_ADDR   = $2F40;              // $036 (  54)

{$R 'nrpm.rc'}

const
  dbs2_nrpn:Array[0..8] of byte = (
      $b0, $63, $37, // (last byte in row) NRPM high reg numeber (always the same)
      $b0, $62, $07, // (last byte in row) NRPM low reg number
      $b0, $06, $7f  // (last byte in row) Register value
  );

// Volumetric settings
  NRPN_EQ_Low       = $00;
  NRPN_EQ_MLow      = $01;
  NRPN_EQ_MHigh     = $02;
  NRPN_EQ_High      = $03;

  NRPN_MasterVolume = $07;

  NRPN_EQ_Low_Cut   = $08;
  NRPN_EQ_MLow_Cut  = $09;
  NRPN_EQ_MHigh_Cut = $0A;
  NRPN_EQ_High_Cut  = $0B;

  NRPN_GM_Rev_Send  = $15;
  NRPN_GM_Chor_Send = $16;

  NRPN_GM_MIDI_Vol  = $22;
  NRPN_GM_MIDI_Pan  = $23;
  NRPN_Mic_Vol      = $24;
  NRPN_Mic_Pan      = $26;
  NRPN_Mic_Echo_Lev = $28;
  NRPN_Mic_Echo_Time= $29;
  NRPN_Mic_Echo_FB  = $2A;

  NRPN_Spatial_Delay= $2C;
  NRPN_Spatial_Input= $2D;

  NRPN_Echo_S1_vol_l= $30;
  NRPN_Echo_S1_vol_r= $31;
  NRPN_Echo_S2_vol_l= $32;
  NRPN_Echo_S2_vol_r= $33;
  NRPN_Echo_MS_vol_l= $34;
  NRPN_Echo_MS_vol_r= $35;


// Toggle settings
  NRPN_Clipping     = $13;
  NRPN_PostGM       = $18;
  NRPN_PostRevChor  = $1A;
  NRPN_Spatial      = $20;

//
  NRPN_Polyphony    = $5F;  // !! only call DBS2_sendControlEvent by DirectSet (without save registers)
{
  7   6   5   4   3   2   1   0
  -  ECH REV CHR OM  MIC EQ2 EQ1

  REV = 1: Reverb ON         polyphony -13
  CHR = 1: Chorus ON         polyphony -3
  MIC = 1: Microphone ON     polyphony -1
  ECH = 1: Mic Echo ON       polyphony -3
   OM = 1: Spatial effect on polyphony -1
  EQ2,EQ1: Equalizer:
   0   0 : Off
   1   0 : 2 band EQ         polyphony -4
   1   1 : 4 band EQ         polyphony -8
}

// Default settings of SAM2695 (DreamBlaster S2) Registers
// 255 means that register is not specified in datasheet
var
  nrpn_regs:Array[0..$35] of byte absolute TAB_NRPM_ADDR;

procedure DBS2_sendControlEvent(nrpmLow:Byte); Register; Keep; assembler;

implementation

procedure DBS2_sendControlEvent(nrpmLow:Byte); Register; Keep; assembler;
asm
event = $FF

// X-reg : pass NRPM Low byte of register to send
  ldy nrpmLow
  lda adr.nrpn_regs,y // get NRPM Register value
directSet:
  sty adr.dbs2_nrpn+5 // store NRPM register number in event data
  sta adr.dbs2_nrpn+8 // store value in event data

  ldx #0
eventSendLoop:
  lda adr.dbs2_nrpn,x
  sta event
  jsr $2006
  inx
  cpx #9
  bmi eventSendLoop
end;

end.