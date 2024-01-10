CFLAGS = -x
asms = dlist dlist_min nrpm_registers

all: displaylists resources

displaylists: ${wildcard dlist*.asm}
	@for i in $(asms); do \
	echo "\ncompile $$i..."; \
	(${CASM} $$i.a65 ${CFLAGS} -o:$$i.obj); done

resources: graph.hex uvmeter.hex help.hex
	@echo
	${XXD} graph.hex graph.obj
	${XXD} uvmeter.hex uvmeter.obj
	${XXD} uvmeter-h.hex uvmeter-h.obj
	${XXD} help158.hex help.obj
