CFLAGS = -x
asms = dlist dlist_min nrpm_registers

all: displaylists resources

displaylists: ${wildcard dlist*.asm}
	@for i in $(asms); do \
	echo "compile $$i..."; \
	(${CASM} $$i.a65 ${CFLAGS} -o:$$i.obj); done

resources: graph.hex uvmeter.hex help.hex
	${XXD} graph.hex graph.obj
	${XXD} uvmeter.hex uvmeter.obj
	${XXD} uvmeter-h.hex uvmeter-h.obj
	${XXD} help158.hex help.obj
