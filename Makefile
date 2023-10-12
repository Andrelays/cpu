assembler_dir 	 = assembler
disassembler_dir = disassembler
cpu_dir          = cpu

release:
	cd $(assembler_dir) 	&& make release
	cd $(cpu_dir)       	&& make release
	cd $(disassembler_dir)  && make release

clean:
	cd $(assembler_dir) 	&& make clean
	cd $(cpu_dir)       	&& make clean
	cd $(disassembler_dir)  && make clean


