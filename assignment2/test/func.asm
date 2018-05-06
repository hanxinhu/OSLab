section .data
color_red: db 1Bh,'[31;1m',0;
.len equ $ - color_red
color_blue: db 1Bh,'[34;1m',0;
.len equ $ - color_blue
color_green: db 1Bh,'[37;0m',0;
.len equ $ - color_green
global  printDirname
global  printFilename;
global printStr;
section .text
printDirname:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_red
	mov edx, color_red.len
	int 80h
	mov eax,4;
	mov ebx,1;
	mov ecx,[esp+4];
	mov edx,[esp+8];
	int 80h;
	ret;

printFilename:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_blue
	mov edx, color_blue.len
	int 80h
 	mov eax,4;
	mov ebx,1;
	mov ecx,[esp+4];
	mov edx,[esp+8];
	int 80h;

	ret;
printStr:
	mov eax, 4;
	mov ebx, 1;
	mov ecx, color_green
	mov edx, color_green.len
	int 80h
 	mov eax,4;
	mov ebx,1;
	mov ecx,[esp+4];
	mov edx,[esp+8];
	int 80h;
	ret;
	