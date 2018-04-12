section .data;
color_red: db 1Bh,'[31;1m',0;
.len equ $ - color_red
color_blue: db 1Bh,'[34;1m',0;
.len equ $ - color_blue
color_default: db 1Bh,'37;0m',0
.len equ $ -color_default

section .bss
temple: resb 33;

section .data


section .text
global _start
_start: 


	mov rax,0;
	mov rbx,1;
	mov rdx,0;
	
Bigloop: 
	add rdx ,1; 
	
	
fibonacii:
	mov rcx ,rbx;
	add rbx, rax;
	mov rax, rcx;
	push rax;
	push rbx;
	push rcx;
	push rdx;
	
	cmp rdx, 10;


	jb iprint;
	jmp exit;



iprint:	; change number to string and print
	mov rcx, 10;
	mov rsi, 32;
	mov byte[temple+rsi],0ah;
	;mov rax,rdx; rax is beichushu
loop:
	sub rsi,1;
	mov rdx, 0;
	div rcx;
	add dl,48;
	mov byte[temple+rsi],dl;
	cmp rax,0
	jne loop
	jmp output;
	
output:
	mov eax,4
	mov ebx,1
	mov ecx,temple
	mov edx,33;
	int 80h

	pop rdx;
	pop rcx;
	pop rbx;
	pop rax;

	jmp Bigloop;

exit:
	mov ebx, 0
	mov eax, 1;
	int 80h

change_blue: 
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_blue
	mov edx, color_blue.len
	int 80h
change_red:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_red
	mov edx, color_red.len
	int 80h
	jmp Bigloop;
change_default:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_default
	mov edx, color_default.len
	int 80h