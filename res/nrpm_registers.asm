    opt h-
    org 0

    // $00 (LSB of NRPN#)
    dta $60    // eq low band              (  +6dB)
    dta $40    // eq med low band          (   0dB)
    dta $40    // eq med hid band          (   0dB)
    dta $60    // eq high band             (  +6dB)
    dta 255
    dta 255
    dta 255
    dta $7f    // master volume            ( 100% )
    dta $0c    // eq low cutoff freq       ( 440Hz)
    dta $1b    // eq med low cutoff freq   ( 885Hz)
    dta $72    // eq med high cutoff freq  (3740Hz)
    dta $40    // eq high cutoff freq      (9375Hz)
    dta 255
    dta 255
    dta 255
    dta 255
    // $10
    dta 255
    dta 255
    dta 255
    dta $00    // clipping mode select                   ( soft )
    dta 255
    dta $40    // General MIDI reverb send               (  50% )
    dta $40    // General MIDI chorus send               (  50% )
    dta 255
    dta $7f    // post effects applied on GM             (   on )
    dta $00    // post effects applied on Mike           (  off )
    dta $7f    // post effects applied on Reverb/Chorus  (   on )
    dta 255
    dta 255
    dta 255
    dta 255
    dta 255
    // $20
    dta $00    // Spatial Effect volume    (   0% )
    dta 255
    dta $7f    // General MIDI volume      ( 100% )
    dta $40    // General MIDI pan         ( midl )
    dta $40    // Mike volume              (  50% )
    dta 255
    dta $40    // Mike pan                 ( midl )
    dta 255
    dta $7f    // Mike Echo level          ( 100% )
    dta $2b    // Mike Echo time           ( 127ms)
    dta $42    // Mike Echo feedback       (  51% )
    dta 255
    dta $1d    // Spatial Effect delay     (      )
    dta $00    // Spatial Effect input     (stereo)
    dta 255
    dta 255
    // $30
    dta $00    // Slave1 Echo volume right (   0% )
    dta $00    // Slave1 Echo volume left  (   0% )
    dta $00    // Slave2 Echo volume right (   0% )
    dta $00    // Slave2 Echo volume left  (   0% )
    dta $7f    // Master Echo volume right ( 100% )
    dta $7f     // Master Echo volume left  ( 100% )
