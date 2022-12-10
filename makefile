CPAS = mp
CASM = mads
XXD = xxd -r -p
MPBASE = ${HOME}/Atari/MadPascal/base

DRVPATH = ./drvs
BINPATH = ./bin
ASMPATH = ./asm

DRIVER ?= MIDIBox

EXEFILE = mcp

SUBDIRS = drvs loader res

MAKE = make
MFLAGS = 'CPAS=${CPAS}' 'CASM=${CASM}' 'XXD=${XXD}' 'BINPATH=../${BINPATH}' 'ASMPATH=../${ASMPATH}'
CFLAGS = -x -l -t

all: prepare compile link
	@echo "\nSzah mat"

prepare:
	@for i in $(SUBDIRS); do \
	echo "make all in $$i..."; \
	(cd $$i; $(MAKE) ${MFLAGS} all); done

compile: compile_pas compile_asm

compile_pas: MCP.pas
	${CPAS} MCP.pas -ipath:./ -define:USE_FIFO -code:8000 -data:0400 -o:${ASMPATH}/MCP.a65

compile_asm: ${ASMPATH}/MCP.a65
	${CASM} ${ASMPATH}/MCP.a65 ${CFLAGS} -i:${MPBASE} -o:${BINPATH}/mcp.bin

link: ${BINPATH}/loader.bin ${BINPATH}/mcp.bin
	rm -vf ${BINPATH}/${EXEFILE}.exe

ifdef DRIVER
	@echo "Linking with driver..."
	cat ${BINPATH}/loader.bin ${BINPATH}/${DRIVER}.drv ${BINPATH}/mcp.bin >> ${BINPATH}/${EXEFILE}.exe
else
	@echo "Linking without driver..."
	cat ${BINPATH}/loader.bin ${BINPATH}/mcp.bin >> ${BINPATH}/${EXEFILE}.exe
endif

clean:
	rm -vf $(BINPATH)/*.bin
