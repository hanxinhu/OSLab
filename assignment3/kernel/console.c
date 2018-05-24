
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			      console.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
	回车键: 把光标移到第一列
	换行键: 把光标前进到下一行
*/


#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#define tabSize 8
#define tab '\0'

PRIVATE void set_cursor(unsigned int position);
PRIVATE void set_video_start_addr(u32 addr);
PRIVATE void flush(CONSOLE* p_con);

/*======================================================================*
			   init_screen
 *======================================================================*/
PUBLIC void init_screen(TTY* p_tty)
{
	int nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;

	int v_mem_size = V_MEM_SIZE >> 1;	/* 显存总大小 (in WORD) */

	int con_v_mem_size                   = v_mem_size / NR_CONSOLES;
	// p_tty->p_console->original_addr      = 0;
	p_tty->p_console->original_addr      = nr_tty * con_v_mem_size;
	p_tty->p_console->v_mem_limit        = con_v_mem_size;
	p_tty->p_console->current_start_addr = p_tty->p_console->original_addr;

	/* 默认光标位置在最开始处 */
	p_tty->p_console->cursor = p_tty->p_console->original_addr;

	if (nr_tty == 0) {

		for (int i = 0; i < 80 * 25; ++i)
		{
			out_char(p_tty->p_console, '\0');
			/* code */
		}
		// /* 第一个控制台沿用原来的光标位置 */
		p_tty->p_console->cursor = disp_pos / 2;
		disp_pos = 0;
	}
	else {
		out_char(p_tty->p_console, nr_tty + '0');
		out_char(p_tty->p_console, '#');
	}

	set_cursor(p_tty->p_console->cursor);
}


/*======================================================================*
			   is_current_console
*======================================================================*/
PUBLIC int is_current_console(CONSOLE* p_con)
{
	return (p_con == &console_table[nr_current_console]);
}
PUBLIC void returnToDefault(CONSOLE* p_con){
 u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);
 u8* p_vmem_base = (u8*)(V_MEM_BASE);
 for (int i = 0; i < p_con->cursor; ++i){
 	*(p_vmem_base+2*i+1) = DEFAULT_CHAR_COLOR;
 }
};
PUBLIC void searchStr(CONSOLE* p_con, char * p,int length){
 u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);
 u8* p_vmem_base = (u8*)(V_MEM_BASE);
 for (int i = 0; i < p_con->cursor - length; ++i)
 {
 	if (*(p_vmem_base+2*i)==*p){
 		int isEqual = 1;
 		for (int j = 0; j < length; ++j)
 		{
 			if (*(p_vmem_base+2*(i+j))!=*(p+j))
 			{	
 				isEqual = 0;
 				break;
 			}

 		}
 		if (isEqual)
 		{
 			for (int j = 0; j < length; ++j)
 			{
 				*(p_vmem_base+2*(i+j)+1) = CHAR_BLUE;
 			}
 		}
 	}
 	
 }
};
/*======================================================================*
			   out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE* p_con, char ch)
{
	u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);

	switch(ch) {
	case '\n':
		if (p_con->cursor < p_con->original_addr +
		    p_con->v_mem_limit - SCREEN_WIDTH) {
			p_con->cursor = p_con->original_addr + SCREEN_WIDTH * 
				((p_con->cursor - p_con->original_addr) /
				 SCREEN_WIDTH + 1);

		}
			// {
			// 	for (int i = 0; i < 128; ++i)
			// 	{
			// *p_vmem++ = i;
			// *p_vmem++ = DEFAULT_CHAR_COLOR;
			// p_con->cursor++;
			// 	}
			
			// }
			
		break;
	case '\b':
		// clearALL(p_con);		
		if (p_con->cursor > p_con->original_addr) {
			if(*(p_vmem-2)==tab && p_con->cursor % SCREEN_WIDTH != 0)
			{
				while(*(p_vmem-2) == tab && p_con->cursor % SCREEN_WIDTH != 0){
					*(p_vmem-2) = '\0';
					*(p_vmem-1) = DEFAULT_CHAR_COLOR;
					p_con->cursor--;
					p_vmem -= 2;
				}
			}
			else if (*(p_vmem-2)=='\0' && p_con->cursor % SCREEN_WIDTH == 0){
					int i = 0;
					while(*(p_vmem-2)=='\0'){
					*(p_vmem-2) = '\0';
					*(p_vmem-1) = DEFAULT_CHAR_COLOR;
					p_con->cursor--;
					p_vmem -= 2;
					i++;
					if(i >= SCREEN_WIDTH)
						break;
				}
			}
					
			else{

			p_con->cursor--;
			*(p_vmem-2) = ' ';
			*(p_vmem-1) = DEFAULT_CHAR_COLOR;
			}
		}
		break;
	case '\t':
		{int leftSize = 0;
		leftSize =  p_con->cursor % SCREEN_WIDTH;
		if(*(p_vmem - 80*2 )==' ' || *(p_vmem - 80*2) == tab)
		for (int i = 0; i < tabSize && i < SCREEN_WIDTH - leftSize ; ++i)
		{
			if(*(p_vmem - 80*2 )==' ' || *(p_vmem - 80*2) == tab){
				*p_vmem++ = tab;
				*p_vmem++ = DEFAULT_CHAR_COLOR; 
				p_con->cursor++;
			}else{
				break;
			}
		
		}
		else{
			for (int i = 0; i < tabSize && i < SCREEN_WIDTH - leftSize ; ++i)
		{
				*p_vmem++ = tab;
				*p_vmem++ = DEFAULT_CHAR_COLOR; 
				p_con->cursor++;
		}
		}
	}
		break;
	default:
		if (p_con->cursor <
		    p_con->original_addr + p_con->v_mem_limit - 1) {
			*p_vmem++ = ch;
			*p_vmem++ = DEFAULT_CHAR_COLOR;
			p_con->cursor++;
		}
		break;
	}

	while (p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE) {
		scroll_screen(p_con, SCR_DN);
	}

	flush(p_con);
}
PUBLIC void out_char_blue(CONSOLE* p_con, char ch)
{
	u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);

	switch(ch) {
	case '\n':
		// if (p_con->cursor < p_con->original_addr +
		//     p_con->v_mem_limit - SCREEN_WIDTH) {
		// 	p_con->cursor = p_con->original_addr + SCREEN_WIDTH * 
		// 		((p_con->cursor - p_con->original_addr) /
		// 		 SCREEN_WIDTH + 1);
		// }
		break;
	case '\b':
		// clearALL(p_con);		
		if (p_con->cursor > p_con->original_addr) {
			p_con->cursor--;
			*(p_vmem-2) = '\0';
			*(p_vmem-1) = CHAR_BLUE;
		}
		break;
	default:
		if (p_con->cursor <
		    p_con->original_addr + p_con->v_mem_limit - 1) {
			*p_vmem++ = ch;
			*p_vmem++ = CHAR_BLUE;
			p_con->cursor++;
		}
		break;
	}

	while (p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE) {
		scroll_screen(p_con, SCR_DN);
	}

	flush(p_con);
}
/*======================================================================*
			   out_char_bul
 *======================================================================*/

/*======================================================================*
                           flush
*======================================================================*/
PRIVATE void flush(CONSOLE* p_con)
{
        set_cursor(p_con->cursor);
        set_video_start_addr(p_con->current_start_addr);
}
PUBLIC void clearALL(CONSOLE* p_con){
	u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);
	while (p_con->cursor >
		    (p_con -> original_addr + 80)) {
			*(p_vmem-2) = '\0';
			*(p_vmem-1) = DEFAULT_CHAR_COLOR;
			p_vmem -= 2;
			p_con->cursor--;
			flush(p_con);
		}
}

/*======================================================================*
			    set_cursor
 *======================================================================*/
PRIVATE void set_cursor(unsigned int position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}

/*======================================================================*
			  set_video_start_addr
 *======================================================================*/
PRIVATE void set_video_start_addr(u32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}



/*======================================================================*
			   select_console
 *======================================================================*/
PUBLIC void select_console(int nr_console)	/* 0 ~ (NR_CONSOLES - 1) */
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) {
		return;
	}

	nr_current_console = nr_console;

	set_cursor(console_table[nr_console].cursor);
	set_video_start_addr(console_table[nr_console].current_start_addr);
}

/*======================================================================*
			   scroll_screen
 *----------------------------------------------------------------------*
 滚屏.
 *----------------------------------------------------------------------*
 direction:
	SCR_UP	: 向上滚屏
	SCR_DN	: 向下滚屏
	其它	: 不做处理
 *======================================================================*/
PUBLIC void scroll_screen(CONSOLE* p_con, int direction)
{
	if (direction == SCR_UP) {
		if (p_con->current_start_addr > p_con->original_addr) {
			p_con->current_start_addr -= SCREEN_WIDTH;
		}
	}
	else if (direction == SCR_DN) {
		if (p_con->current_start_addr + SCREEN_SIZE <
		    p_con->original_addr + p_con->v_mem_limit) {
			p_con->current_start_addr += SCREEN_WIDTH;
		}
	}
	else{
	}

	set_video_start_addr(p_con->current_start_addr);
	set_cursor(p_con->cursor);
}

