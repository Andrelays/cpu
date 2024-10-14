assembler_dir 	 = assembler
disassembler_dir = disassembler
cpu_dir          = cpu

release:
	cd $(assembler_dir) 	&& make all
	cd $(cpu_dir)       	&& make all
	cd $(disassembler_dir)  && make all

clean:
	cd $(assembler_dir) 	&& make clean
	cd $(cpu_dir)       	&& make clean
	cd $(disassembler_dir)  && make clean



