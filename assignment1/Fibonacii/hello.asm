section .data;
color_red: db 1Bh,'[31;1m',0;
.len equ $ - color_red
color_blue: db 1Bh,'[34;1m',0;
.len equ $ - color_blue
color_default: db 1Bh,'37;0m',0
.len equ $ -color_default

message: db "hello World",0Ah,0Ah ; 0Ah for nextline
var: db 215
section .bss
temple: resb 33;

section .text
global _start
_start: 
	

	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_blue
	mov edx, color_blue.len
	int 80h
	jmp hdiv;

loop:

	mov eax, 48
	add eax, edi;

		mov byte[temple+9],48 
	 	mov byte[temple+10] ,0ah;;
	mov ecx, temple
	add edi, 1;
	cmp edi,esi;
	jne output;
	jmp exit;

output: 
	mov edx,33
	mov eax,4
	mov ebx,1
	mov ecx,temple
	int 80h
	jmp exit;

hdiv:
	mov edx ,[var]
	mov byte[temple+32],0ah;
	mov ecx, 32;
divloop:
	mov eax, 2;
	div 10;

	sub ecx,1; ;counts;
	
	cmp eax,0;
	mov edx, eax;
	add al , 48;
	mov byte[temple+ecx],al;
	mov eax, 10;
	jne divloop;
	jmp output;
	

exit:
	mov ebx, 0
	mov eax, 1;
	int 80h
