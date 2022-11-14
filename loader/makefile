FILENAME = loader
CFLAGS = -x -l:${ASMPATH}/${FILENAME}.lst -t:${ASMPATH}/${FILENAME}.lab

all: loader

loader: ${FILENAME}.a65
	${CASM} ${FILENAME}.a65 ${CFLAGS} -o:${BINPATH}/${FILENAME}.bin
