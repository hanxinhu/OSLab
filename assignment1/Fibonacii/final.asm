section .bss;
var1: resb 64;
var2: resb 64;
var3: resb 64;
temple: resb 65;
strs: resb 64;
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
.len equ $ - color_purple

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
	mov edx,64;
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
	mov rax,var1;
	mov byte[rax+63],0;
	mov rax,var2;
	mov byte[rax+63],1;

	

	mov rsi,0; ;for counts;
	pop rcx;
	;mov rcx,5; for high
	pop rdx;
	;mov rdx,0; for low egde
init:
	cmp rsi,rdx;
	jae output;
fi:	
	jmp fibonacii;
inherit:
	add rsi,1;
	cmp rsi,rcx;
	ja exit;
	jmp init;

fibonacii:
	push rax;
	push rbx;
	push rcx;
	push rdx;
	push rsi;
	; var3 = var2;
	mov rcx,64;
loop1: 
	sub rcx,1; 
	mov bl,[var2+rcx];
	mov [var3+rcx],bl;
	cmp rcx,0;
	jne loop1;

	; var2 = var1 + var2;
	mov rdx,0;
	mov rcx,64;
addloop:
	sub rcx,1;

	mov bl,[var1+rcx];
	mov al,[var2+rcx];
	add al,bl;
	add al,dl;
	cmp al,10;
	jae WithCarry;
	mov dl,0;
	mov [var2+rcx],al;
	cmp rcx,0;
	je step3;
	jmp addloop;
WithCarry:
	mov dl,1;
	sub al,10;
	mov [var2+rcx],al;
	jmp addloop;
	;var1 = var3;
step3:
	mov rcx,64;
loop2: 
	sub rcx,1; 
	mov bl,[var3+rcx];
	mov [var1+rcx],bl;
	cmp rcx,0;
	jne loop2;
	pop rsi;
	pop rdx;
	pop rcx;
	pop rbx;
	pop rax;
	jmp inherit;

	

output:
	push rax;
	push rbx;
	push rcx;
	push rdx;
	push rsi;
	mov rcx,64;
	mov rax,temple;


outputLoop:
	sub rcx,1; 
	mov bl,[var1+rcx];
	add bl,48;
	mov [temple+rcx],bl;
	cmp rcx,0;
	jne outputLoop;

	mov rcx,0;
qingchu:
	mov bl,[temple+rcx];
	cmp bl,48;
	jne put;
	mov byte[temple+rcx],0h
	add rcx ,1;
	cmp rcx ,63;
	jne qingchu;
put:	
	mov byte[temple+64],0ah;
	mov eax,4
	mov ebx,1
	mov ecx,temple
	mov edx,65;
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
	jmp fi;


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