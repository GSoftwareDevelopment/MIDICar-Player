# Memory usage

## Zero Page

| Address              | Type    | Name            | Function                                                     | Source    |
| -------------------- | ------- | --------------- | ------------------------------------------------------------ | --------- |
| $d4, $d5             | pointer | `scradr`        | position on screen                                           | main      |
| $d6,$d7              | pointer | nn              | UVMeter                                                      | main      |
|                      |         |                 |                                                              |           |
| $dc                  | byte    | `curTrackPtr`   | pointer to current proceeded track                           | midfiles  |
| $de                  | byte    | `cTrk`          | id of current proceeded track                                | midfiles  |
| $df                  | byte    | `playingTracks` | number of tracks currently being processed<br />Zero means the song has ended. | midfiles  |
| $f0-$f3              | word    | `_totalTicks`   | master timer                                                 | midfiles  |
| $f4                  | byte    | `_subCnt`       | sub-counter for master timer                                 | midfiles  |
| $f5                  | byte    | `_timerStatus`  | main timer status                                            | midfiles  |
| $f6-$f9              | long    | `_delta`        |                                                              | midfiles  |
| $f6                  | byte    | `_tmp`          | first byte of `_delta`                                       | midfiles  |
|                      |         |                 |                                                              |           |
| $e0 (_trkRegs+0)     | byte    | `_status`       | track status                                                 | midfiles  |
| $e1 (_trkRegs+1)     | byte    | `_bank`         | extended memory bank index                                   | midfiles  |
| $e2-$e3 (_trkRegs+2) | pointer | `_ptr`          | data pointer                                                 | midfiles  |
| $e2 (_trkRegs+2)     | word    | `_adr`          | data address (word)                                          | midfiles  |
| $e4-$e7 (_trkRegs+4) | longint | `_trackTime`    | the time at which the track will be played                   | midfiles  |
| $e8 (_trkRegs+8)     | byte    | `_event`        | last MIDI event                                              | midfiles  |
| $e9 (_trkRegs+9)     | byte    | `_volume`       | last NOTE ON velocity                                        | midfiles  |
|                      |         |                 |                                                              |           |
| $ff                  | byte    | `MC_Byte`       | data for and from MC6850                                     | mc6850    |
|                      |         |                 |                                                              |           |
| $fd                  | byte    | `FIFO_Head`     |                                                              | midi_fifo |
| $fe                  | byte    | `FIFO_Tail`     |                                                              | midi_fifo |
| $ff                  | byte    | `FIFO_Byte`     | data for and from FIFO                                       | midi_fifo |



## Buffers

| Start | Size | Name       | Function                        | Source    |
| ----- | ---- | ---------- | ------------------------------- | --------- |
| $0600 | 256  | `FIFO_Buf` | FIFO Buffer                     | midi_fifo |
| $0600 | 256  | `RBuf`     | I/O buffer for MID file loading | midfiles  |



## Data

| Address     | Size | Name           | Function                                                     | Source   |
| ----------- | ---- | -------------- | ------------------------------------------------------------ | -------- |
| $3000       | 1024 | `CHARS_ADDR`   | Char set definition                                          | main     |
| $3400       | 34   | `DLIST_ADDR`   | Display List                                                 | main     |
| $3430       | 1040 | `SCREEN_ADDR`  | Screen data                                                  | main     |
| $3840       | 64   | `UVMETER_ADDR` | Channel indicator data                                       | main     |
|             |      |                |                                                              |          |
| $3C00       | 512  |                | Track data right after loading the file<br />fast return to the beginning of the track |          |
| $3E00       | 512  |                | Tracks information                                           | midfiles |
| $4000-$7fff |      |                | Song data                                                    | midfiles |
| $a400-$cfff |      |                | Song data                                                    | midfiles |
| $d800-$feff |      |                | Song data                                                    | midfiles |

