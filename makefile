all: clox

clox:
	gcc -Wall main.c compiler.c scanner.c vm.c object.c value.c memory.c chunk.c debug.c -o clox

clean:
	rm clox
