section .bss;
var1: resb 33;
var2: resb 33;
var3: resb 33;
temple: resb 33;
section .text;
global _start;
_start:
	mov rax,var1;
	mov byte[rax+32],1;
	mov rax,var2;
	mov byte[rax+32],1;
	mov rcx, 0;
OuterLoop:
	add rcx,1;
	cmp rcx,3;
	

	je exit;
fibonacii:
	; var3 = var2;
	push rcx;
	mov rcx,33;
loop1: 
	sub rcx,1; 
	mov bl,[var2+rcx];
	mov [var3+rcx],bl;
	cmp rcx,0;
	jne loop1;

	; var2 = var1 + var2;
	mov rcx,33;
	mov rdx,0;
loop3:	
	
	sub rcx,1;
	cmp rcx, 0;
	je step3;
	mov bl,[var1+rcx];
	mov al,[var2+rcx];
	add al,[var2+rcx];
	add al,dl;
	cmp al,10;
	jae carryOpe;
	mov dl,0;

	mov [var2+rcx],al;
	jmp loop3;
carryOpe:	
	mov dl,1
	sub al,10;
	mov byte[var2+rcx],al;
	jmp loop3;

step3:
	; var1 = var3;
	mov rcx,33;
loop2: 
	sub rcx,1; 
	mov bl,[var3+rcx];
	mov [var+rcx],bl;
	cmp rcx,0;
	jne loop2;

	mov rcx,33;
	mov rax,temple;



outputLoop:
	sub rcx,1; 
	mov bl,[var1+rcx];
	add bl,48;
	mov [temple+rcx],bl;
	cmp rcx,0;
	jne outputLoop;

	mov byte[temple+32],0ah;
	mov eax,4
	mov ebx,1
	mov ecx,temple
	mov edx,33;
	int 80h
	pop rcx;
	jmp outputLoop;


exit:
	mov ebx, 0
	mov eax, 1;
	int 80h