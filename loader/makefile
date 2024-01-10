FILENAME = loader
CFLAGS = -x -l:${ASMPATH}/${FILENAME}.lst -t:${ASMPATH}/${FILENAME}.lab

all: loader decomp

loader: ${FILENAME}.a65
	${CASM} ${FILENAME}.a65 ${CFLAGS} -o:${BINPATH}/${FILENAME}.bin

decomp: decomp.a65
	${CASM}	decomp.a65 -x -l:${ASMPATH}/decomp.lst -t:${ASMPATH}/decomp.lab -o:${BINPATH}/decomp.bin
