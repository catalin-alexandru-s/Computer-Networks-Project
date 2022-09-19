all:
	gcc servtcp.c -o servtcp.bin
	gcc cl.c -o cl.bin
sv:
	./servtcp.bin

cl:
	./cl.bin 127.0.0.1 2024

