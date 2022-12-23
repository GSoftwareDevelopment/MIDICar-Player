![MIDICar Logo](MIDICar Logo.png)

**MIDICar Player** is a MID files player and was created in cooperation with **MIDICar** interface creator (*Błażej "Pancio" Biernat*) to be connected to CART/ECI or PBI connector of 8-bit Atari.

## Evolution

1.2 - Sparta DOS X support
1.5 - Supports other MIDI devices, including: MIDIMate and MIDIBox, using the SIO connector. This is supported by the drivers included with the software.

## Using

The program offers simple interface and intuitive operation from the computer keyboard. Most of the control is done with `arrow keys` and the `Return` key. A help screen is also available, which is displayed by pressing the `H` or `HELP` key.

The layout of the playback controls are taken from WinAMP and are respectively:

- `X` - Play
- `C` - Pause
- `V` - Stop
- `Z` and `B` - for changing the track Previous and Next respectively.

The looping mode can be changed with the `L` key and is done sequentially:

- Single track,
- One track in loop,
- Entire list in loop (123)
- and Shuffle.

The `SPACE` key gives the option to unhook from the track list. These will be skipped in "123" and Shuffle modes, and when switching tracks with the `Z` and `V` keys.

It is possible to change the speed of the track being played. Using the `<`, `>` keys which decrease and increase the playback speed respectively, and `Back Space` which restores the original speed of the song.

You can also change the colour scheme of the player from light to dark with the `INVERS` key.

The `M` key can be used to turn off the channel volume preview, which can have some effect on song playback.

Exit the program by pressing the `ESC` key - the program does not confirm this operation.

## Requirements

- 64KB of RAM, where approximately 26KB will be available per song, hence memory expansion to at least 128KB is recommended
- the program only supports PORTB-based extensions, e.g. Compy, Rambo, U1MB.
- DOS MEMLO must be below $2000
- no use of RAM under ROM and extended memory - data placed in RAM disks may be destroyed!

# Command Line Option

## Syntax

`MCP [[D[n]:][path]midifile.ext] [/D[n]:[path]driver.ext] [/Rh] [/Ccolours] [/Shex]`

Specifying a file specification results in it being passed on to the player, which then automatically loads and plays it. In this situation, the programme enters SINGLE-SONG mode.
The file browser is switched off.
The file specification, does not necessarily have to be at the beginning (as the first parameter) it may as well be at the end or between switches.

ONLY ONE file can be passed to the player.

## Switches

/D - specifies the MIDI driver file.

**IMPORTANT NOTE**
It is required to specify the full path to th driver file. The letter `D` is both a switch and part of the device name. There is no support for a one-letter device specification, This must be specified as `D1:` to `DO:`.

/R - Allows you to specify, how often (every screen frame) the information will be refreshed.

/C - Defines the player colours.

Specify the hexadecimal values for the individual colour registers (708 to 712) as the `colours` parameter. The first five numbers, specify the first set of colours, the next five, the second set.
Sets can be switched, using the **INVERS** key.

/S - Allows you to define your own SysEx message of the MIDI Reset type.

**QUICK NOTE**

This message is called every time the player stops playing. Its function is to reset the MIDI device, including muting the sounds played, channel settings, etc. etc.
The message definition consists of a sequence of bytes, represented in hexadecimal. In theory, this string can be up to 256 bytes long; in practice, this message should be no longer than a dozen bytes.
