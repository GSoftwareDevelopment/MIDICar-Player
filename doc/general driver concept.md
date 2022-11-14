# General driver concept for MIDICar Player

A description of how to build a driver for MIDICar Player is given below.

The code presented below is for the MADS Assembler compiler.

## Lets start

~~~assembler

    org $2000
~~~

The driver __must__ be loaded at address $2000 and __must__ start with a Jump Table!

Generally, there is 2KB allocated for the driver which should be completely sufficient.

~~~assembler
;-------------------
; Driver Jump Table

drvjtab:
  jmp driver.init
  jmp driver.setup
  jmp driver.send
  jmp driver.flush
  jmp driver.shutdown
~~~

## Initialize section

This section is designed to detect the device.

Also, it is a good idea to include here the code responsible for displaying the driver message.

~~~assembler
  .local driver

;------------
; Initialize

DESC	dta c'Driver name',$9B

init:

; Show info about driver

  lda #<DESC
  ldy #>DESC
  jsr PRINT

;

  rts
~~~

## Setup section

This section must contain everything needed for the device to initialize properly.

- store the original vectors, the change of which is required for the correct operation of the device
- initialization of the FIFO buffer.

~~~assembler
;---------------
; Setup driver

setup:

  rts
~~~

## Send byte section

This section is responsible for sending one byte, the value of which is passed in the Accumulator.

Here, FIFO buffer support is also provided since the MPC, by itself, does not support such a buffer.

~~~assembler
;-------------------------
; Send one byte by driver

Send:

  rts
~~~

## Flush section

The primary task of the section is to wait until the device sends all the information.

**Needed if a FIFO buffer is used!**

This is also the polling hook for MCP and should recognize the Carry flag, according to the scheme:

- Carry Set - try to send a byte if the device is ready
- Catty Clear - wait until the device sends all the information

~~~assembler
;--------------
; Flush buffer

Flush:

  rts
~~~

## Shut down section

The section responsible for shutting down the device.

- Completing the sending of data (for example, from the FIFO buffer).
- restoring any modified vectors, set in the initialization process

The section should not send MIDI reset data - this is handled by MCP

~~~assembler
;------------------
; Shut down driver

ShutDown:

  rts
~~~

## External procedures

~~~assembler
;----------------------------
; External procedures

PRINT:

    .local

ICCHID  = $0340
ICCMD   = $0342
ICBUFA  = $0344
ICBUFL  = $0348
CIOV    = $E456

    ldx #$00
    sta ICBUFA,X
    tya
    sta ICBUFA+1,X
    lda #$ff
    sta ICBUFL,X
    lda #$09
    sta ICCMD,X
    lda ICCHID,x
    bmi ExitPRINT
    jmp CIOV
ExitPRINT:
    rts

    .endl
~~~

## Variables section

~~~assembler
;------------------
; Driver variables

  .endl
~~~

## Run driver

The compiler should create a runnable file.

~~~assembler
; Run driver

  ini driver.init
~~~

## That's all