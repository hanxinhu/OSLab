%define precison 40
section .bss
var: resb 40;  store 32 bytes
section .text
global _start
_start:
	mov eax, 3
	mov ebx, 1
	mov ecx, var
	mov edx, precison
	int 80h

	mov eax, 4
	mov ebx, 1
	mov ecx, var
	mov edx, precison
	int 80h

	mov eax, 1
	mov ebx, 0
	int 80h
