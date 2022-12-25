# General driver concept for MIDICar Player

A description of how to build a driver for MIDICar Player.

It is worth studying with the code of the drivers that have already been written to get a good understanding of their design and functionality.

The code presented below is for the MADS Assembler compiler.

## Lets start

~~~assembly

    org $2000
    
~~~

The driver __must__ be loaded at address $2000 and __must__ start with a Jump Table!

Generally, there is 2KB allocated for the driver which should be completely sufficient.

~~~assembly
;-------------------
; Driver Jump Table

  .local jumpTable
  
    jmp driver.init
    jmp driver.setup
    jmp driver.send
    jmp driver.flush
    jmp driver.shutdown
  
DESC	dta c'Driver name',$9B

  .endl
~~~



## Driver Main block

```assembly
  .local driver
```

## Setup section

This section must contain everything needed for the device to initialize properly.

- store the original vectors, the change of which is required for the correct operation of the device
- initialization of the FIFO buffer.

~~~assembly
;---------------
; Setup driver

  .local Setup

	rts
	
  .endl
~~~

## Send byte section

This section is responsible for sending one byte, the value of which is passed in the Accumulator.

Here, FIFO buffer support is also provided since the MPC, by itself, does not support such a buffer.

~~~assembly
;-------------------------
; Send one byte by driver

  .local Send

	fifo@put
	
StartSend:

    rts
  
  .endl
~~~

## Flush section

The primary task of the section is to wait until the device sends all the information.

**Needed if a FIFO buffer is used!**

This is also the polling hook for MCP and should recognize the Carry flag, according to the scheme:

- Carry Set - try to send a byte if the device is ready
- Catty Clear - wait until the device sends all the information

~~~assembly
;--------------
; Flush buffer

  .local Flush

    bcc Send.startSend
  
All:

    rts
  
  .endl
~~~

## Shut down section

The section responsible for shutting down the device.

- Completing the sending of data (for example, from the FIFO buffer).
- restoring any modified vectors, set in the initialization process

The section should not send MIDI reset data - this is handled by MCP

~~~assembly
;------------------
; Shut down driver

  .local ShutDown

  rts
  
  .endl
~~~

## Variables section

~~~assembly
;------------------
; Driver variables

~~~

## Initialize section

This section is designed to detect the device, and is only called once, during driver initialisation from the loader block.
The reason why it is placed at the end of the code is so that once the controller is correctly initialised, this code can be overwritten - it is not used later in the main program.

Also, it is a good idea to include here the code responsible for displaying the driver message.

~~~assembly
;------------
; Initialize

  .local init

; Show info about driver

    lda #<DESC
    ldy #>DESC
    jsr PRINT

	sec
	
	rts
~~~

The above section should return a set flag `C` when the device has been correctly detected. Otherwise, this flag should be cleared.

### External procedures

All the functions and procedures needed in the initiation process are well placed in this section.

~~~assembly
;----------------------------
; External procedures

  .local PRINT

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

    rts

  .endl
~~~

## Finish

Close section `Init`, and main section `Driver`

```assembly
  .endl

  .endl
```



## That's all