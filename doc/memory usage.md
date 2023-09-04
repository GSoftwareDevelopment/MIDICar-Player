# Memory usage

## Zero Page

| Address    | Alternate    | Type    | Name            | Function                                                     | Source    |
| ---------- | ------------ | ------- | --------------- | ------------------------------------------------------------ | --------- |
| `12`       |              | byte    | `tm`            |                                                              | main      |
| `13`       |              | byte    | `otm`           |                                                              | main      |
| `$58, $59` |              | pointer | `scradr`        | position on screen                                           | main      |
| `$D2`      |              | byte    | `refreshRate`   |                                                              | main      |
| `$D3`      |              | byte    | `totalXMS`      |                                                              | loader    |
| `$D8`      |              | byte    | `playerStatus`  |                                                              | main      |
| `$D9`      |              | byte    | `screenStatus`  |                                                              | main      |
| `$DA..$DB` |              | pointer | `listPtr`       | Playlist pointer                                             | main      |
| `$DC..$DD` |              | pointer | `curTrackPtr`   | pointer to current proceeded track                           | midfiles  |
| `$DE`      |              | byte    | `cTrk`          | id of current proceeded track                                | midfiles  |
| `$DF`      |              | byte    | `playingTracks` | number of tracks currently being processed<br />Zero means the song has ended. | midfiles  |
| `$F0-$F3`  |              | word    | `_totalTicks`   | master timer                                                 | midfiles  |
| `$F4`      |              | byte    | `_subCnt`       | sub-counter for master timer                                 | midfiles  |
| `$F5`      |              | byte    | `_timerStatus`  | main timer status                                            | midfiles  |
| `$F6-$F9`  |              | long    | `_delta`        |                                                              | midfiles  |
| `$F6`      |              | byte    | `_tmp`          | first byte of `_delta`                                       | midfiles  |
|            |              |         |                 |                                                              |           |
| `$E0`      | (_trkRegs+0) | byte    | `_bank`         | extended memory bank index                                   | midfiles  |
| `$E1-$E2`  | (_trkRegs+1) | pointer | `_ptr`          | data pointer                                                 | midfiles  |
| `$E1`      | (_trkRegs+1) | word    | `_adr`          | data address (word)                                          | midfiles  |
| `$E3-$E6`  | (_trkRegs+3) | longint | `_trackTime`    | the time at which the track will be played                   | midfiles  |
| `$E7`      | (_trkRegs+7) | byte    | `_eStatusRun`   |                                                              | midfiles  |
|            |              |         |                 |                                                              |           |
| `$E8`      |              | byte    | `_tickStep`     |                                                              | midfiles  |
| `$E9`      |              | long    | `_songTicks`    | Song size in ticks                                           | midfiles  |
|            |              |         |                 |                                                              |           |
| `$FF`      |              | byte    | `MC_Byte`       | data for and from MC6850                                     | mc6850    |
|            |              |         |                 |                                                              |           |
| `$FD`      |              | byte    | `FIFO_Head`     |                                                              | midi_fifo |
| `$FE`      |              | byte    | `FIFO_Tail`     |                                                              | midi_fifo |
| `$FF`      |              | byte    | `FIFO_Byte`     | data for and from FIFO                                       | midi_fifo |

## Variables

| Address        |         | Type               | Name        | Description | source    |
| -------------- | ------- | ------------------ | ----------- | ----------- | --------- |
| `$14`          |         | Byte               | `_tm`       |             | main      |
| `$13`          |         | Byte               | otm         |             | main      |
| `$12`          |         | Byte               | ctm         |             | main      |
| `$D4`          |         | Byte               | OSDTm       |             | main      |
|                |         |                    |             |             |           |
| `$D5`          |         | Byte               | chn         |             | main      |
| `$D6`          |         | Byte               | oldV        |             | main      |
| `$D7`          |         | ShortInt<br />Byte | v<br />_v   |             | main      |
|                |         |                    |             |             |           |
| `$5A`          |         | Byte               | lstY        |             |           |
| `$5B`          |         | SmallInt           | lstShift    |             |           |
| `$70`          |         | SmallInt           | lstCurrent  |             |           |
| `$74`          |         | SmallInt           | lstTotal    |             |           |
| `$72`          |         | SmallInt           | curPlay     |             |           |
|                |         |                    |             |             |           |
| `$D7`          |         | Byte               | ilch        |             | inputLine |
| `$54`          |         | Byte               | ilpos       |             | inputLine |
| `$55`          |         | Word               | ilscradr    |             | inputLine |
|                |         |                    |             |             |           |
| `$88`          |         | Long               | counter     |             | main      |
| `$8c`          |         | Long               | cntBCD      |             | main      |
|                |         |                    |             |             |           |
| `$0400..$048A` |         | MCP Variables      |             |             | MCP       |
|                |         |                    |             |             |           |
| `$4DC..$4DF`   | 4       | DWord              | fnExt       |             | filestr   |
| `$4e0..$4ef`   | 16      | array              | COLORS_ADDR |             | main      |
| `$4f0..$4f7`   | 6 (+1)  | TDevString         | curDev      |             | filestr   |
| `$4f8..$538`   | 64 (+1) | TPath              | curPath     |             | filestr   |
| `$539..$559`   | 32 (+1) | TFilename          | fn          |             | filestr   |
|                |         |                    |             |             |           |

## Buffers

| Start        | Size | Name   | Function      | Source  |
| ------------ | ---- | ------ | ------------- | ------- |
| `$55A..$5AA` | 80   | outstr |               | filestr |
| `$5AB..$5FB` | 80   | snull  |               | filestr |
| `$600..$6FF` | 256  |        | MIDI-Out FIFO | driver  |
|              |      |        |               |         |

## Data

| Address            |            Size | Name             | Function                                                     | Source                 |
| ------------------ | --------------: | ---------------- | ------------------------------------------------------------ | ---------------------- |
| `$2380..$23FF`     |       128 ($80) | `KEY_TABLE_ADDR` | Keys jump table                                              | main                   |
| `$2400..$27FF`     |     1024 ($400) | `CHARS_ADDR`     | Char set definition                                          | main                   |
|                    |                 |                  |                                                              |                        |
| **`$2800..$2E03`** | **1780 ($6F4)** |                  | **Screen data**                                              | **main**               |
| `$2800..$2AF7`     |      760 ($2F8) | SCREEN_HEAD      | Screen header                                                |                        |
| `$2AF8..$2B1F`     |       40 ($028) | SCREEN_FOOT      | Screen footer                                                |                        |
| `$2B20..$2CFF`     |      720 ($2D0) | SCREEN_WORK      | Screen Work Area                                             |                        |
| `$2D00..$2D77`     |      120 ($078) | SCREEN_CHANNELS  | Screen Channels View                                         |                        |
| `$2D78..$2D83`     |       60 ($03C) | SCREEN_TIME      | Screen control & time                                        |                        |
| `$2D84..$2DAB`     |       40 ($028) | SCREEN_TIMELINE  | Screen song time line progress                               |                        |
| `$2DAC..$2DE7`     |       60 ($03C) | SCREEN_OSD       | Screen for OSD                                               | main                   |
| `$2ED8..$2E10`     |       40 ($028) | SCREEN_STATUS    | Screen status line                                           |                        |
|                    |                 |                  |                                                              |                        |
| `$2EA0..$2EDF`     |        64 ($40) | `UVMETER_ADDR`   | Channel indicator data                                       | main                   |
| `$2EE0..$2EFF`     |        32 ($20) | `SCREEN_ADRSES`  | listScrAdr                                                   | main                   |
|                    |                 |                  |                                                              |                        |
| `$2F00..$2F3F`     |        64 ($40) | `TAB_SCAN2ASC`   | keyboard code conversion table to ascii codes                | main (keyscan2asc.a65) |
| `$2F40..$2F75`     |        54 ($35) | `NRPM_REGS`      | DreamBlaster s2 NRPM Registers                               | nrpm                   |
| `$2F75..$2FB3`     |        62 ($3E) | `DLIST_MIN_ADDR` | Display List for MinMode                                     | main                   |
| `$2FB4..$2FFF`     |        75 ($4B) | `DLIST_ADDR`     | Display List                                                 | main                   |
|                    |                 |                  |                                                              |                        |
| `$3000..$32CF`     |      720 ($2D0) | `HELPSCR_ADDR`   | Screen for help                                              | main                   |
|                    |                 |                  |                                                              |                        |
| `$3B00..$3BFF`     |      256 ($100) | SYSEX_MSG        | MIDI Reset System Exclusive Message                          | midfiles               |
| `$3C00`            |             512 |                  | Tracks data right after loading the file<br />fast return to the beginning of the track | midfiles               |
| `$3E00`            |             512 |                  | Tracks information                                           | midfiles               |
|                    |                 |                  |                                                              |                        |
| `$80..$D3`         |                 | ZPAGE            | MADPascal ZP variables                                       | EXE                    |
| `$8000..$BFFF`     |                 | CODE             | MADPascal executable code                                    | EXE                    |
|                    |                 |                  |                                                              |                        |
| `$4000..$7FFF`     |                 | `LIST_ADDR`      | List buffer<br />(in base memory)                            | main                   |
|                    |                 |                  |                                                              |                        |
|                    |                 |                  |                                                              |                        |

# Driver

| Address        | Size       | Name       | Function    |
| -------------- | ---------- | ---------- | ----------- |
| `$0600`        | 256        | `FIFO_Buf` | FIFO Buffer |
| `$2000..$22FF` | 768 ($300) |            | MIDI Driver |
