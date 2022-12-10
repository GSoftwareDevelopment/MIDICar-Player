DRIVERS = MIDIBox MIDIMate MIDICar

all: ${wildcard *.a65}
	@for i in $(DRIVERS); do \
	echo "compile driver $$i..."; \
	(${CASM} $$i.a65 -x -l:${ASMPATH}/$$i.lst -t:${ASMPATH}/$$i.lab -o:${BINPATH}/$$i.drv); done
