#!/bin/bash

echo "# Prepeare resources..."
cd resources
./make.sh
cd ..

echo "# Compiling main program..."
mp 'MIDICar Player.pas' -ipath:./ -define:USE_FIFO -code:8000 -data:0400 -o:'./asm/MIDICar Player.a65'
mads './asm/MIDICar Player.a65' -x -l -t -i:$HOME/Atari/MadPascal/base -o:./bin/p.com

echo "# Makeing loader..."
cd loader
./make.sh
cd ..

echo "# Linking..."
cd bin
./make.sh
cd ..
