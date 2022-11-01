# Memory usage

## Zero Page

| Address    | Alternate    | Type    | Name            | Function                                                     | Source    |
| ---------- | ------------ | ------- | --------------- | ------------------------------------------------------------ | --------- |
| `12`       |              | byte    | `tm`            |                                                              | main      |
| `13`       |              | byte    | `otm`           |                                                              | main      |
| `4A`       |              | byte    | `playerStatus`  |                                                              | loader    |
| `4B`       |              | byte    | `totalXMS`      |                                                              | loader    |
| `4F`       |              | byte    | `puttextinvert` |                                                              | screen (lib) |
| `$D4, $D5` |              | pointer | `scradr`        | position on screen                                           | main      |
| `$D6, $D7` |              | pointer | *n/n*           | help pointer                                                 | main      |
| `$D8, $D9` |              | pointer | `MCBase`        | MC6850 base address                                          | loader    |
| `$DA..$DB` |              | pointer | `pls`           | Playlist pointer                                             | main      |
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
| `$E9` | | long | `_songTicks` | Song size in ticks | midfiles |
| | | | | | |
| `$FF`      |              | byte    | `MC_Byte`       | data for and from MC6850                                     | mc6850    |
|            |              |         |                 |                                                              |           |
| `$FD`      |              | byte    | `FIFO_Head`     |                                                              | midi_fifo |
| `$FE`      |              | byte    | `FIFO_Tail`     |                                                              | midi_fifo |
| `$FF`      |              | byte    | `FIFO_Byte`     | data for and from FIFO                                       | midi_fifo |



## Variables

| Address        |         | Type               | Name       | Description | source    |
| -------------- | ------- | ------------------ | ---------- | ----------- | --------- |
| `$D6`          |         | Byte               | chn        |             | main      |
| `$D7`          |         | ShortInt<br />Byte | v<br />_v  |             | main      |
|                |         |                    |            |             |           |
| `$3540..$355F` |         | Array of Word      | listScrAdr |             | main      |
| `$D6`          |         | Byte               | ilch       |             | inputLine |
| `$54`          |         | Byte               | ilpos      |             | inputLine |
| `$55`          |         | Word               | ilscradr   |             | inputLine |
|                |         |                    |            |             |           |
| `$88`          |         | Long               | counter    |             | main      |
| `$8c`          |         | Long               | cntBCD     |             | main      |
|                |         |                    |            |             |           |
| `$4f0..$4f7`   | 6 (+1)  | TDevString         | curDev     |             | main      |
| `$4f8..$538`   | 64 (+1) | TPath              | curPath    |             | main      |
| `$539..$559`   | 32 (+1) | TFilename          | fn         |             | main      |
|                |         |                    |            |             |           |



## Buffers

| Start          | Size  | Name       | Function                        | Source    |
| -------------- | ----- | ---------- | ------------------------------- | --------- |
| `$55A..$5AA`   | 80    | outstr     |                                 | main      |
| `$5AB..$5FB`   | 80    | snull      |                                 | main      |
| `$0600`        | 256   | `FIFO_Buf` | FIFO Buffer                     | midi_fifo |
| `$4000..$7FFF` | 16384 | `RBuf`     | I/O buffer for MID file loading | midfiles  |



## Data

| Address        | Size        | Name             | Function                                                     | Source   |
| -------------- | ----------- | ---------------- | ------------------------------------------------------------ | -------- |
| `$3000..$33FF` | 1024 ($400) | `CHARS_ADDR`     | Char set definition                                          | main     |
| `$3FAC..$3FFF` |             | `DLIST_ADDR`     | Display List                                                 | main     |
| `$2000..$26F3` | 1780 ($6F4) | `SCREEN_ADDR`    | Screen data                                                  | main     |
| `$3400..$347F` | 128 ($80)   | `KEY_TABLE_ADDR` | Keys jump table                                              | main     |
| `$3500..$353F` | 64          | `UVMETER_ADDR`   | Channel indicator data                                       | main     |
|                |             |                  |                                                              |          |
| `$2C00`        | 512         |                  | Tracks data right after loading the file<br />fast return to the beginning of the track |          |
| `$2E00`        | 512         |                  | Tracks information                                           | midfiles |
|                |             |                  |                                                              |          |
| `$80..$D3`     |             | ZPAGE            | MADPascal ZP variables                                       | EXE      |
| `$8000..$AB21` |             | CODE             | MADPascal executable code                                    | EXE      |
| `$0400..$0484` |             | DATA             | MADPascal Static Data                                        | EXE      |
|                |             |                  |                                                              |          |
|                |             |                  |                                                              |          |
| `$4000..$7FFF` |             |                  | Song data                                                    | midfiles |
| `$D800..$FF00` |             |                  | Song data                                                    | midfiles |

