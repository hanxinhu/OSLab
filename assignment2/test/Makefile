all:
	nasm -felf -o func.o func.asm
	gcc -m32 main.c func.o
	./a.out