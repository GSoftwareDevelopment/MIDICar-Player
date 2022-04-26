 //Please set base address for MC6850
//
// IOPORT_base_address - should be defined in main.asm
//

MC6850BASE  equ IOPORT_base_address

;VIA6522 registers
MC6850_CNTRREG  equ MC6850BASE + 0    ;    Control Register (write access)
MC6850_BUFFER   equ MC6850BASE + 1    ;    Receive/Send Buffer
MC6850_STATREG  equ MC6850BASE + 0    ;    Status Register (read access)



//MC6850_CNTRREG Write Access
CR0 equ 1       ;   Counter Divide Select Bit0    00 = f/1      01 = f/16
CR1 equ 2       ;   Counter Divide Select Bit1    10 = f/64     11 = Master Reset
CR2 equ 4       ;   Word Select Bit0
CR3 equ 8       ;   Word Select Bit1
CR4 equ 16      ;   Word Select Bit2
CR5 equ 32      ;   Transmitter Control Bit0
CR6 equ 64      ;   Transmitter Control Bit1
CR7 equ 128     ;   Receive Interrupt Enable

//MC6850_CNTRREG Read Access
RDRF    equ 1       ;   Receive Data Register full - set to LOW when data read by CPU. Set HIGH when data ready to read from buffer.
TDRE    equ 2       ;   Transmit Data Register Empty - set HIGH when send buffer capable to send next byte.
DCD     equ 4       ;   Data Carrier Detect (LOW active)
CTS     equ 8       ;   Clear To Send (active LOW)
FE      equ 16      ;   Framming Error (active HIGH)
OVRN    equ 32      ;   Receiver Overrun(active HIGH)
PE      equ 64      ;   Parity Error (active HIGH)
IRQ     equ 128     ;   Interrupt Request - state of the /IRQ output, set HIGH when IRQ appeared. Crear automaticale when read/write to buffer.




