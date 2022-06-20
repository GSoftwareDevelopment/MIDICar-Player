# Memory usage

## Zero Page

| Address    | Alternate    | Type    | Name            | Function                                                     | Source    |
| ---------- | ------------ | ------- | --------------- | ------------------------------------------------------------ | --------- |
| `4A`       |              | byte    | playerStatus    |                                                              | main      |
| `$D4, $D5` |              | pointer | `scradr`        | position on screen                                           | main      |
| `$D6, $D7` |              | pointer | *n/n*           | help pointer                                                 | main      |
| `$D8, $D9` |              | pointer | MCBase          | MC6850 base address                                          | loader    |
| `$DA..$DB` |              | pointer | pls             | Playlist pointer                                             | main      |
| `$DC..$DD` |              | pointer | `curTrackPtr`   | pointer to current proceeded track                           | midfiles  |
| `$DE`      |              | byte    | `cTrk`          | id of current proceeded track                                | midfiles  |
| `$DF`      |              | byte    | `playingTracks` | number of tracks currently being processed<br />Zero means the song has ended. | midfiles  |
| `$F0-$F3`  |              | word    | `_totalTicks`   | master timer                                                 | midfiles  |
| `$F4`      |              | byte    | `_subCnt`       | sub-counter for master timer                                 | midfiles  |
| `$F5`      |              | byte    | `_timerStatus`  | main timer status                                            | midfiles  |
| `$F6-$F9`  |              | long    | `_delta`        |                                                              | midfiles  |
| `$F6`      |              | byte    | `_tmp`          | first byte of `_delta`                                       | midfiles  |
|            |              |         |                 |                                                              |           |
| `$E0`      | (_trkRegs+0) | byte    | `_status`       | track status                                                 | midfiles  |
| `$E1`      | (_trkRegs+1) | byte    | `_bank`         | extended memory bank index                                   | midfiles  |
| `$E2-$E3`  | (_trkRegs+2) | pointer | `_ptr`          | data pointer                                                 | midfiles  |
| `$E2`      | (_trkRegs+2) | word    | `_adr`          | data address (word)                                          | midfiles  |
| `$E4-$E7`  | (_trkRegs+4) | longint | `_trackTime`    | the time at which the track will be played                   | midfiles  |
| `$E8`      | (_trkRegs+8) | byte    | `_event`        | last MIDI event                                              | midfiles  |
|            |              |         |                 |                                                              |           |
| `$FF`      |              | byte    | `MC_Byte`       | data for and from MC6850                                     | mc6850    |
|            |              |         |                 |                                                              |           |
| `$FD`      |              | byte    | `FIFO_Head`     |                                                              | midi_fifo |
| `$FE`      |              | byte    | `FIFO_Tail`     |                                                              | midi_fifo |
| `$FF`      |              | byte    | `FIFO_Byte`     | data for and from FIFO                                       | midi_fifo |



## Buffers

| Start          | Size  | Name       | Function                        | Source    |
| -------------- | ----- | ---------- | ------------------------------- | --------- |
| `$0600`        | 256   | `FIFO_Buf` | FIFO Buffer                     | midi_fifo |
| `$4000..$7FFF` | 16384 | `RBuf`     | I/O buffer for MID file loading | midfiles  |



## Data

| Address        | Size        | Name           | Function                                                     | Source   |
| -------------- | ----------- | -------------- | ------------------------------------------------------------ | -------- |
| `$3000..$33FF` | 1024 ($400) | `CHARS_ADDR`   | Char set definition                                          | main     |
| `$3FB7..$3FFF` | 74 ($4A)    | `DLIST_ADDR`   | Display List                                                 | main     |
| `$3400..$3AF3` | 1780 ($6F4) | `SCREEN_ADDR`  | Screen data                                                  | main     |
| `$3AF4..$3B34` | 64          | `UVMETER_ADDR` | Channel indicator data                                       | main     |
|                |             |                |                                                              |          |
| `$2C00`        | 512         |                | Tracks data right after loading the file<br />fast return to the beginning of the track |          |
| `$2E00`        | 512         |                | Tracks information                                           | midfiles |
|                |             |                |                                                              |          |
| `$80..$D3`     |             | ZPAGE          | MADPascal ZP variables                                       | EXE      |
| `$8000..$9437` |             | CODE           | MADPascal executable code                                    | EXE      |
| `$3D90..$3E68` |             | DATA           | MADPascal Static Data                                        | EXE      |
|                |             |                |                                                              |          |
|                |             |                |                                                              |          |
| `$4000..$7FFF` |             |                | Song data                                                    | midfiles |
| `$A800..$CFFF` |             |                | Song data                                                    | midfiles |
| `$D800..$FF00` |             |                | Song data                                                    | midfiles |

