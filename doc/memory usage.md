# Memory usage

## Zero Page Variables

| Address    | Alternate    | Type    | Name            | Function                                                     | Source    |     |
| ---------- | ------------ | ------- | --------------- | ------------------------------------------------------------ | --------- | --------- |
| `$12`   |      | Byte               | ctm        |             | main      |       |
| `$13`   |      | Byte               | otm        |             | main      |       |
| `$14`   |      | Byte               | `_tm`      |             | main      |       |
| `$54`   |      | Byte               | ilpos      |             | inputLine |  |
| `$55..$56` |      | Word               | ilscradr   |             | inputLine |  |
| `$57` | | byte | putTextInvert |  | screen |  |
| `$58..$59` |              | pointer | `scradr`        | position on screen                                           | screen |  |
| `$5A`  |      | Byte               | lstOldY    |             |           |           |
| `$5B`   |      | SmallInt           | lstShift   |             |           |           |
| `$60`   |      | Byte               | lstY       |             |           |           |
| `$61` |              | byte    | `refreshRate`   |                                                              | main      | L     |
| `$62` |              | byte    | `totalXMS`      |                                                              | main, loader | L |
| `$70..$71` |      | SmallInt           | lstCurrent |             |           |           |
| `$72..$73` |      | SmallInt           | lstTotal   |             |           |           |
| `$74..$75` |      | SmallInt           | curPlay    |             |           |           |
| `$79..$7A` |      | Pointer            | KEYDEFP    |             | OS        |         |
| `$D4`   |      | Byte               | OSDTm      |             | main      |       |
| `$D5`   |      | Byte               | chn        |             | main      |       |
| `$D6`   |      | Byte               | oldV       |             | main      |       |
| `$D7`   |      | ShortInt<br />Byte | v<br />_v  |             | main      |       |
| `$D6..$D7` | | Word | curLoad | | main.loadSong midfile.loadMID | |
| `$D7`   |      | Byte               | ilch       |             | inputLine |  |
| `$D8`      |              | byte    | `playerStatus`  |                                                              | main      | L     |
| `$D9`      |              | byte    | `screenStatus`  |                                                              | main      | L     |
| `$DA..$DB` |              | pointer | `listPtr`       | Playlist pointer                                             | list  |   |
| `$DC..$DD` |              | pointer | `curTrackPtr`   | pointer to current proceeded track                           | midfiles  |   |
| `$DE`      |              | byte    | `cTrk`          | id of current proceeded track                                | midfiles  |   |
| `$DF`      |              | byte    | `playingTracks` | number of tracks currently being processed<br />Zero means the song has ended. | midfiles  |   |
| `$F0..$F3` |              | long | `_timerTick` | master timer                                                 | midfiles  |   |
| `$F4`      |              | byte    | `_subCnt`       | sub-counter for master timer                                 | midfiles  |   |
| `$F5`      |              | byte    | `_timerStatus`  | main timer status                                            | midfiles  |   |
| `$F6..$F9` |              | long    | `_delta`        |                                                              | midfiles  |   |
| `$F6`      |              | byte    | `_tmp`          | first byte of `_delta`                                       | midfiles  |   |
|            |              |         |                 |                                                              |           |           |
| `$E0`      | (_trkRegs+0) | byte    | `_bank`         | extended memory bank index                                   | midfiles  |   |
| `$E1..$E2` | (_trkRegs+1) | pointer | `_ptr`          | data pointer                                                 | midfiles  |   |
| `$E1..$E2` | (_trkRegs+1) | word    | `_adr`          | data address (word)                                          | midfiles  |   |
| `$E3..$E6` | (_trkRegs+3) | longint | `_trackTime`    | the time at which the track will be played                   | midfiles  |   |
| `$E7`      | (_trkRegs+7) | byte    | `_eStatusRun`   |                                                              | midfiles  |   |
|            |              |         |                 |                                                              |           |           |
| `$E8`      |              | byte    | `_tickStep`     |                                                              | midfiles  |   |
| `$E9..$EC` |              | long    | `_totalSongTicks`    | Song size in ticks                                           | midfiles  |   |
|            |              |         |                 |                                                              |           |           |
| `$FF`      |              | byte    | `MC_Byte`       | data for and from MC6850                                     | mc6850    |     |
|            |              |         |                 |                                                              |           |           |
| `$FD`      |              | byte    | `FIFO_Head`     |                                                              | midi_fifo |  |
| `$FE`      |              | byte    | `FIFO_Tail`     |                                                              | midi_fifo |  |
| `$FF`      |              | byte    | `FIFO_Byte`     | data for and from FIFO                                       | midi_fifo |  |

| Address        |         | Type          | Name        | Description | source  |      |
| -------------- | ------- | ------------- | ----------- | ----------- | ------- | ---- |
| `$0400..$04B4` |         | MCP Variables |             |             | MCP     |      |
|                |         |               |             |             |         |      |
| `$4DC..$4DF`   | 4       | DWord         | fnExt       |             | filestr |      |
| `$4E0..$4Ef`   | 16      | array         | COLORS_ADDR |             | main    | L    |
| `$4F0..$4F7`   | 6 (+1)  | TDevString    | curDev      |             | filestr |      |
| `$4F8..$538`   | 64 (+1) | TPath         | curPath     |             | filestr |      |
| `$539..$559`   | 32 (+1) | TFilename     | fn          |             | filestr |      |
|                |         |               |             |             |         |      |

## Buffers

| Start        | Size | Name   | Function      | Source  |
| ------------ | ---- | ------ | ------------- | ------- |
| `$55A..$5AA` | 80   | outstr |               | filestr |
| `$5AB..$5FB` | 80   | snull  |               | filestr |
| `$600..$6FF` | 256  |        | MIDI-Out FIFO | driver  |
|              |      |        |               |         |

## Data

| Address            |            Size | Name             | Function                                                     | Source           |      |
| ------------------ | --------------: | ---------------- | ------------------------------------------------------------ | ---------------- | ---- |
| `$2300..$237F`     |       128 ($80) | `KEY_TABLE_ADDR` | Keys jump table                                              | keys             |      |
| `$2380..$239F`     |        32 ($20) | `SCREEN_ADRSES`  | listScrAdr                                                   | main             |      |
| `$2FCA..$23FF`     |        54 ($35) | `NRPM_REGS`      | DreamBlaster s2 NRPM Registers                               | nrpm             | E    |
| `$2400..$27FF`     |     1024 ($400) | `CHARS_ADDR`     | Char set definition                                          | main             | E    |
| `$2800..$283F`     |        64 ($40) | `UVMETER_ADDR`   | Channel indicator data                                       | main             | E    |
| `$2840..$288B`     |        75 ($4B) | `DLIST_ADDR`     | Display List                                                 | main             | E    |
| `$288C..$28C3`     |        62 ($3E) | `DLIST_MIN_ADDR` | Display List for MinMode                                     | main             | E    |
|                    |                 |                  |                                                              |                  |      |
| **`$29C0..$2FFF`** | **1600 ($640)** |                  | **Screen data**                                              | **main**         |      |
| `$29C0..$29E7`     |       40 ($028) | SCREEN_FOOT      | Screen footer                                                |                  |      |
| `$29E8..$2BC7`     |      480 ($1E0) | SCREEN_WORK      | Screen Work Area                                             |                  |      |
| `$2BC8..$2C3F`     |      120 ($078) | SCREEN_CHANNELS  | Screen Channels View                                         |                  |      |
| `$2C40..$2C7B`     |       60 ($03C) | SCREEN_TIME      | Screen control & time                                        |                  |      |
| `$2C7C..$2CA3`     |       40 ($028) | SCREEN_TIMELINE  | Screen song time line progress                               |                  |      |
| `$2CA4..$2CDF`     |       60 ($03C) | SCREEN_OSD       | Screen for OSD                                               | osd              |      |
| `$2CE0..$2D07`     |       40 ($028) | SCREEN_STATUS    | Screen status line                                           |                  |      |
| `$2D08..$2FFF`     |      760 ($2F8) | SCREEN_HEAD      | Screen header                                                |                  | E    |
| `$3000..$32CF`     |      720 ($2D0) | `HELPSCR_ADDR`   | Screen for help                                              | main             | E    |
|                    |                 |                  |                                                              |                  |      |
| `$3D00..$3DFF`     |      256 ($100) | SYSEX_MSG        | MIDI Reset System Exclusive Message                          | midfiles, loader | L    |
| `$3E00`            |             512 |                  | Tracks information                                           | midfiles         |      |
|                    |                 |                  |                                                              |                  |      |
| `$80..$D3`         |                 | ZPAGE            | MADPascal ZP variables                                       | EXE              |      |
| `$8000..$BFFF`     |                 | CODE             | MADPascal executable code                                    | EXE              |      |
|                    |                 |                  |                                                              |                  |      |
| `$4000..$7FFF`     |                 | `LIST_ADDR`      | List buffer<br />(in base memory)                            | main             |      |
|                    |                 |                  |                                                              |                  |      |
| `$4000.$41FF`      |             512 |                  | Tracks data right after loading the file<br />fast return to the beginning of the track<br />only in last `totalXMS` bank! | midfiles         |      |
|                    |                 |                  |                                                              |                  |      |

# Driver

| Address        | Size       | Name       | Function    |      |
| -------------- | ---------- | ---------- | ----------- | ---- |
| `$0600..$06FF` | 256        | `FIFO_Buf` | FIFO Buffer |      |
| `$2000..$22FF` | 768 ($300) |            | MIDI Driver | L    |
