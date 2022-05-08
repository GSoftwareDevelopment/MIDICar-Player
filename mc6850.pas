Unit MC6850;

Interface

const
    MC6850_BASE = $D500;

// adresses of VIA6522 registers
    ADDR_MC6850_CNTRREG  = MC6850_BASE + 0;    // Control Register (write access)
    ADDR_MC6850_BUFFER   = MC6850_BASE + 1;    // Receive/Send Buffer
    ADDR_MC6850_STATREG  = MC6850_BASE + 0;    // Status Register (read access)

//MC6850_CNTRREG Write Access
    CR0 = 1       ;   // Counter Divide Select Bit0    00 = f/1      01 = f/16
    CR1 = 2       ;   // Counter Divide Select Bit1    10 = f/64     11 = Master Reset
    CR2 = 4       ;   // Word Select Bit0
    CR3 = 8       ;   // Word Select Bit1
    CR4 = 16      ;   // Word Select Bit2
    CR5 = 32      ;   // Transmitter Control Bit0
    CR6 = 64      ;   // Transmitter Control Bit1
    CR7 = 128     ;   // Receive Interrupt Enable

    CD_1         = %00;
    CD_16        = %01;
    CD_64        = %10;
    MasterReset  = %11;

    WS_7bits     = %00000;
    WS_8bits     = %10000;
    WS_2BitStop  = %00000;
    WS_1BitStop  = %01000;
    WS_EvenParity= %00000;
    WS_OddParity = %00100;

    TC_RTSlow_noTIRQ = %0000000;
    TC_RTSlow_TIRQ   = %0100000;
    TC_RTShigh_noTIRQ= %1000000;
    TC_BreakLevel    = %1100000;

    noIRQ_Receive    = %00000000;
    IRQ_Receive      = %10000000;

//MC6850_CNTRREG Read Access
    RDRF = 1       ;   // Receive Data Register full - set to LOW when data read by CPU. Set HIGH when data ready to read from buffer.
    TDRE = 2       ;   // Transmit Data Register Empty - set HIGH when send buffer capable to send next byte.
    DCD  = 4       ;   // Data Carrier Detect (LOW active)
    CTS  = 8       ;   // Clear To Send (active LOW)
    FE   = 16      ;   // Framming Error (active HIGH)
    OVRN = 32      ;   // Receiver Overrun(active HIGH)
    PE   = 64      ;   // Parity Error (active HIGH)
    IRQ  = 128     ;   // Interrupt Request - state of the /IRQ output, set HIGH when IRQ appeared. Clear automaticale when read/write to buffer.

var
// VIA6522 registers
    MC6850_CNTRREG  : byte absolute ADDR_MC6850_CNTRREG;
    MC6850_BUFFER   : byte absolute ADDR_MC6850_BUFFER;
    MC6850_STATREG  : byte absolute ADDR_MC6850_CNTRREG;
    MC_Byte         : byte absolute $ff;

procedure MC6850_Reset;
procedure MC6850_Init(setup:byte);
function  MC6850_Receive:byte;
procedure MC6850_Send(data:byte);
procedure MC6850_Send2; inline;

implementation

procedure MC6850_Reset;
begin
    MC6850_CNTRReg:=MasterReset;
end;

procedure MC6850_Init(setup:byte);
begin
    MC6850_CNTRReg:=setup;
end;

function MC6850_Receive:byte;
begin
    repeat until (MC6850_CNTRReg and RDRF)<>0;
    result:=MC6850_BUFFER;
end;

procedure MC6850_Send(data:byte);
begin
{$IFDEF DEBUG} poke($d01a,4); {$ENDIF}
    repeat until (MC6850_CNTRReg and TDRE)<>0;
    MC6850_BUFFER:=data;
{$IFDEF DEBUG} poke($d01a,15); {$ENDIF}
end;

procedure MC6850_Send2; inline;
begin
    repeat until (MC6850_CNTRReg and TDRE)<>0;
    MC6850_BUFFER:=MC_Byte;
end;
end.