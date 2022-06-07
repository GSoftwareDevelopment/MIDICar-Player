#!/bin/bash

echo "# Prepeare resources..."
cd resources
./make.sh >> /dev/null
cd ..

echo "# Compiling main program..."
mp 'MIDICar Player.pas' -ipath:./ -define:USE_FIFO -define:USE_CIO -define:USE_SUPPORT_VARS -code:8000 -data:2000 -o:'./asm/MIDICar Player.a65' >> /dev/null
mads './asm/MIDICar Player.a65' -x -l -t -i:$HOME/Atari/MadPascal/base -o:./bin/p.com >> /dev/null

echo "# Makeing loader..."
cd loader
./make.sh >> /dev/null
cd ..

echo "# Linking..."
cd bin
./make.sh >> /dev/null
cd ..
