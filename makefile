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
CFLAGS = -x -l -t -s

all: prepare compile link
	@echo "\nSzah mat"

drivers:
	@cd drvs
	${MAKE} ${MFLAGS} all

prepare:
	@rm -vf ./logs/*.log
	@for i in $(SUBDIRS); do \
	echo "--- make all in $$i..."; \
	(cd $$i; $(MAKE) ${MFLAGS} all >> ../logs/$$i.log); done

compile: compile_pas compile_asm

compile_pas: MCP.pas
	${CPAS} MCP.pas -ipath:./ -define:USE_FIFO -define:USE_SUPPORT_VARS -code:8000 -data:0400 -o:${ASMPATH}/MCP.a65 >> ./logs/mp.log

compile_asm: ${ASMPATH}/MCP.a65
	${CASM} ${ASMPATH}/MCP.a65 -m:../res/macros.a65 ${CFLAGS} -i:${MPBASE} -o:${BINPATH}/mcp.bin >> ./logs/mads.log

link: ${BINPATH}/loader.bin ${BINPATH}/mcp.bin
	@rm -vf ${BINPATH}/${EXEFILE}.exe >> ./logs/link.log

ifdef DRIVER
	echo "Linking with driver..."
	cat ${BINPATH}/${DRIVER}.drv ${BINPATH}/loader.bin ${BINPATH}/mcp.bin >> ${BINPATH}/${EXEFILE}.exe
else
	echo "Linking without driver..."
	cat ${BINPATH}/loader.bin ${BINPATH}/mcp.bin >> ${BINPATH}/${EXEFILE}.exe
endif

clean:
	@rm -vf $(BINPATH)/*.bin
	@rm -vf ./logs/*.log
