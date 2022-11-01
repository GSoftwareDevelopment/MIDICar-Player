# Introduction

The repository contains source code for a MIDI file player designed for the ATARI 8-bit platform.
It uses the MIDICar interface plugged in as an external device, via the ECI/CARD bus (XE series) or the PBI bus (XL series).

# Language

The programming language used is MAD Pascal compiler and MADS Assembler compiler for computers with 6502 processor.

# Requirements
In order to run the program correctly, an ATARI platform with a minimum of **64KB RAM** is required. Such a configuration will allow loading a MIDI file with a very small size, as **29KB** of RAM will be available. To load larger files without hindrance, it is a good idea to equip your computer with a minimum of **256KB RAM**.

# DOS

The player works with (practically) any known DOS, but as a developer I recommend using DOS whose **MEMLO** is no higher than **$2000**, and the RAM area under the ROM remains at the user's disposal.
In addition, the use of **RAM-DISK** is not advisable, as the data contained therein can be tampered with by the program.

Users using **SpartaDOS/SDX** can also use the player without much hindrance. The only requirement is to configure the system to use extended memory (`USE BANKED` configuration).

# Compilation
The program consists of two parts: the loader and the actual program.
Both are compiled separately.
The loader should be compiled using MADS Assembler. A BASH script is available in the `loader` directory, with which, you can compile the source code of the loader.

Use MAD Pascal and MADS Assembler to compile the actual program.
```
mp 'MIDICar Player.pas' -ipath:./ -define:USE_FIFO -code:8000 -data:0400 -o:'./asm/MIDICar Player.a65'
mads './asm/MIDICar Player.a65' -x -l -t -i:$HOME/Atari/MadPascal/base -o:./bin/p.com
```

The two parts need to be combined into a single file, e.g. using command (linux):
```
cat loader.com p.com >> mcplay.exe
```

The resulting file created in this way, you can run.

# Documentation

Instructions on how to use the program are included in the document `docs/MIDICar Player Description.md`.

A small summary, related to the program's memory usage, is also available. The whole in the document `dosc/memory usage.md`.

At: https://miro.com/app/board/uXjVOo1LIvc=/?share_link_id=542607287697 a block diagram of the program - mainly of the MID player part - is also available.

# Contributions

Błażej "Pancio" Biernat

All PTODT Users

# Licence

Copyright (c) 2021-2022 GSD

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