
section .bss
temple: resb 33;
strs: resb 33;
section .data
string: db "91111";
section .text
global _start
_start:

	mov eax,3
	mov ebx,1
	mov ecx,strs;
	mov edx,33;
	int 80h

	mov rcx,0;
	mov rdx,10;
	mov rax,0;
	mov rbx,0;
	mov rsi,0;
	jmp read;



read:
	mov bl, byte[strs+rcx];
	cmp bl, 47;
	jb 	readnext;
	cmp bl, 58;
	ja  readnext;
 	mul rdx;
	mov rdx,10;
	sub bl,48;
	add rax,rbx;
next:	
	add rcx,1;
	jmp read;
readnext:
	push rax;
	add rsi,1;
	cmp rsi,2;
	je  exact;
	
	mov rdx,10;
	mov rax,0;
	mov rbx,0;
	jmp next;

	
exact:
	pop rax;
	


	
iprint:	; change number to string and print
	mov rcx, 10;
	mov rsi, 32;
	mov byte[temple+rsi],0ah;
	;mov rax,rdx; rax is beichushu
divloop:
	sub rsi,1;
	mov rdx, 0;
	div rcx;
	add dl,48;
	mov byte[temple+rsi],dl;
	cmp rax,0
	jne divloop
	jmp output;
	
output:
	mov eax,4
	mov ebx,1
	mov ecx,temple
	mov edx,33;
	int 80h


exit:

	mov ebx, 0
	mov eax, 1;
	int 80h