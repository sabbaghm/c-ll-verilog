all:
	make -C parser all
	make -C codegen all
clean:
	make -C parser clean
	make -C codegen clean
