section .bss
temple: resb 33;
strs: resb 33;
section .data
tip: db "please input x and y:"
.len equ $ - tip
color_red: db 1Bh,'[31;1m',0;
.len equ $ - color_red
color_blue: db 1Bh,'[34;1m',0;
.len equ $ - color_blue
color_white: db 1Bh,'[37;1m',0;
.len equ $ - color_white
color_green: db 1Bh,'[32;1m',0;
.len equ $ - color_green
color_yellow: db 1Bh,'[33;1m',0;
.len equ $ - color_yellow
color_purple db 1Bh,'[35;1m',0;
.len equ $ - color_purple;
section .text
global _start

_start: 
	mov eax,4
	mov ebx,1;
	mov ecx,tip;
	mov edx,21;
	int 80h

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
	mov rax,0;
	mov rbx,1;
	mov rsi,0; ;for counts;
	pop rcx;
	;mov rcx,10; for high
	pop rdx;
	;mov rdx,0; for low egde
	

loop:
	
	cmp rsi,rdx;
	jae iprint;
	

	jmp fibonacii;
inherit:
	add rsi,1;
	cmp rsi,rcx;
	ja exit;


	jmp loop;

	
	
fibonacii:
	push rcx;
	mov rcx ,rbx;
	add rbx, rax;
	mov rax, rcx;
	pop rcx;
	jmp inherit;
	


	



iprint:	; change number to string and print
	push rax;
	push rbx;
	push rcx;
	push rdx;
	push rsi;

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
	
	 pop rsi;
	 push rsi;

	mov rax, rsi;
	mov rdx,0;
	mov rsi,6;
	div rsi;
	
	cmp rdx,1;
	je change_red;
	cmp rdx,2;
	je change_blue;
	cmp rdx,0;
	je change_default;
	cmp rdx,3
	je change_purple;
	cmp rdx,4
	je change_green;
	cmp rdx,5;
	je change_yellow;

continue:
	pop rsi;
	pop rdx;
	pop rcx;
	pop rbx;
	pop rax;
	jmp fibonacii;

exit:
	mov ebx, 0
	mov eax, 1;
	int 80h

change_red:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_red
	mov edx, color_red.len
	int 80h
	jmp continue;

change_blue: 
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_blue
	mov edx, color_blue.len
	int 80h
	jmp continue;


change_default:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_white
	mov edx, color_white.len
	int 80h
	jmp continue;
change_purple:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_purple
	mov edx, color_purple.len
	int 80h
	jmp continue;
change_yellow:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_yellow
	mov edx, color_yellow.len
	int 80h
	jmp continue;
change_green:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_green
	mov edx, color_green.len
	int 80h
	jmp continue;