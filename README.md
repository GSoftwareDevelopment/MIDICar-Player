![MIDICar logo](/doc/MIDICar%20Logo.png)

[About MIDICar](https://github.com/GSoftwareDevelopment/MIDICar-Player)
[About MIDICar-Remote](https://github.com/GSoftwareDevelopment/MIDICar-Player)

# Introduction

The repository contains source code for a MIDI file player designed for the ATARI 8-bit platform.
It uses the MIDICar interface plugged in as an external device, via the ECI/CARD bus (XE series) or the PBI bus (XL series).

# Language

The programming language used is MAD Pascal compiler and MADS Assembler compiler for computers with 6502 processor.

# Requirements

In order to run the program correctly, an ATARI platform with a minimum of **64KB RAM** is required. Such a configuration will allow loading a MIDI file with a very small size, as **29KB** of RAM will be available. To load larger files without hindrance, it is a good idea to equip your computer with a minimum of **256KB RAM**.

# DOS

The player works with (practically) any known DOS, but as a developer I recommend using DOS  with Command Processor like:

- Sparta Dos X - best option :P
- Sparta Dos
- BW DOS
- DOS II+ D
- DOS XL
- realDOS
- XDOS - but loader can't recognize CP

**MEMLO** must be no higher than **$2000**, and the RAM area under the ROM remains at the user's disposal.
In addition, the use of **RAM-DISK** is not advisable, as the data contained therein can be tampered with by the program.

Users using **SpartaDOS/SDX** can also use the player without much hindrance. The only requirement is to configure the system to use extended memory (`USE BANKED` configuration).

Any other DOS may cause greater or lesser startup problems - this has not been tested.
In case the DOS in use does not have the ability to pass a parameter to the program, it is important to load the appropriate MIDI driver before running the program.

# Run

It is important to indicate on the command line which driver will be used. To do this, run the programme as follows:

Type:

**`MCPLAY.EXE /D`**`x`**`:`**`path>filename.ext`

- in place of `x` enter the device number, or skip it
- `path>` - path can only be specified in DOS that supports directories like Sparta DOS, Sparta DOS X, MyDos.
- `filename.ext` driver file name

**The program supports ONLY `Dx:` disk devices.**

If your DOS does not have the ability to pass parameters to the program, the driver MUST be loaded into memory before running. Load it as you would a normal EXE or COM binary file.

# Drivers

Drivers are currently available:

- **MIDICar** - PBI or ECI/CARD based MIDI expanded card with independent communication port

SIO devices:

- **MIDIMate** - which has its own internal 31.25KHz clock
- **MIDIBox** - clocked by a clock generated by POKEY

- These are bootable, non-relocatable files.
- They do not set MEMLO, so they can be overwritten!
- They load in the area from $2000 onward. After running, a corresponding message should appear on the screen (the name of the driver)

More about the drivers in the document `general drivers concept.md` in the `/doc` directory.

## Bundling

There may be problems loading the driver from under DOS that do not have a Command Line with parameter passing. The solution to this is to be able to link the driver together with the main program.

The important thing with this operation is that the main program is CONNECTED TO the driver, not the other way around.

The linking operation can be performed on a PC using (under Linux) the command:

`cat driver MCP.EXE >> mcpbundl.exe`.

in place of `driver` specify the name of the driver file. Of course, the name of the resulting file can be different :)

# Compilation

Type `make` in main directory.

Use `make -DRIVER={file_name}` to connect the driver to the program.
The names of the drivers are available in the `drvs` directory - all files with the `a65` extension.
Do not specify the extension in the `DRIVER=` parameter.

# Documentation

Instructions on how to use the program are included in the document `docs/MIDICar Player Description.md`.

A small summary, related to the program's memory usage, is also available. The whole in the document `dosc/memory usage.md`.

At: https://miro.com/app/board/uXjVOo1LIvc=/?share_link_id=542607287697 a block diagram of the program - mainly of the MID player part - is also available.

# Contributions

- Błażej "Pancio" Biernat
- Jerzy "Mono" Kut

All PTODT Users

# If you like this...

[![ByMeCaffee](../../../GSoftwareDevelopment/raw/main/bmc.png)](https://www.buymeacoffee.com/PeBe)

# Licence

MIT Licence by GSoftDev 2021-2022

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.