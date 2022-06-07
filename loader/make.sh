#!/bin/bash
MPBase="$HOME/Atari/MadPascal/base"

#mp loader.pas -ipath:./ -code:8000 -o:./asm/loader.a65
#mads ./asm/loader.a65 -x -l -t -i:$MPBase -o:../bin/loader.com
mads ./loader.a65 -x -l -t -o:../bin/loader.com
