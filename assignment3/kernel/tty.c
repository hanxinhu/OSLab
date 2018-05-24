
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               tty.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
#include <stdlib.h>


#define TTY_FIRST	(tty_table)
#define TTY_END		(tty_table + NR_CONSOLES)

PRIVATE void init_tty(TTY* p_tty);
PRIVATE void tty_do_read(TTY* p_tty);
PRIVATE void tty_do_write(TTY* p_tty);
PRIVATE void put_key(TTY* p_tty, u32 key);
PRIVATE int isSearch;
PRIVATE char search[32] = {0} ;
PRIVATE int isEnter;
PRIVATE int length;

/*======================================================================*
                           task_tty
 *======================================================================*/
CONSOLE* p_con;

PUBLIC void task_tty()
{
	TTY*	p_tty;

	init_keyboard();
	isSearch = 0;
	isEnter = 0;
	for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
		init_tty(p_tty);
	}
	isEnter = 0;
	length = 0;
	select_console(0);
	
	
	while (1) {		
		 for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
		 	tty_do_read(p_tty);
		 	tty_do_write(p_tty);
		 	p_con = p_tty -> p_console;
		 }
	}
}

PUBLIC void clean(){
	
	while(1)
	{	
		if(p_con !=NULL)
			if(!isSearch)
			{
				clearALL(p_con);
			}	
		milli_delay(20 * 1000);
	}
}
/*======================================================================*
			   init_tty
 *======================================================================*/
PRIVATE void init_tty(TTY* p_tty)
{
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}

/*======================================================================*
				in_process
 *======================================================================*/
PUBLIC void in_process(TTY* p_tty, u32 key)
{
        char output[2] = {'\0', '\0'}	;
       	
        if (!(key & FLAG_EXT)) {
        	if (!(isSearch && isEnter ))
        	{put_key(p_tty, key);
        	if (isSearch & !isEnter)
        	search[length++] = key;
        	}
        }
        else {
            int raw_code = key & MASK_RAW;
            switch(raw_code) {
            	case ENTER:
            	if (!isSearch)
			 		put_key(p_tty, '\n');
				else
					{
					isEnter = 1;
					searchStr(p_tty->p_console,search,length);
					
					}	
			break;
                case BACKSPACE:
                	if(!isEnter)
					put_key(p_tty, '\b');
					if(isSearch & !isEnter)
					if(length>0)
					{
						length--;
					}
			break;
			case ESC:
				if(isSearch)
				{	
					if (isEnter)

					{
					isSearch = 0;	
					isEnter = 0;
					for (int i = 0; i < length; ++i)
					{
						put_key(p_tty, '\b');
					}
					returnToDefault(p_tty->p_console);
					length = 0;
					}		
				}
				else
					isSearch = 1;
				break;
			break;
                case UP:
                        if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_DN);
                        }
			break;
			case DOWN:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_UP);
			}
			break;
			case TAB:
					put_key(p_tty, '\t');
					break;
            default:
                      break;
            }

        }
}

/*======================================================================*
			      put_key
*======================================================================*/
PRIVATE void put_key(TTY* p_tty, u32 key)
{
	if (p_tty->inbuf_count < TTY_IN_BYTES) {
		*(p_tty->p_inbuf_head) = key;
		p_tty->p_inbuf_head++;
		if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_head = p_tty->in_buf;
		}
		p_tty->inbuf_count++;
	}
}


/*======================================================================*
			      tty_do_read
 *======================================================================*/
PRIVATE void tty_do_read(TTY* p_tty)
{
	if (is_current_console(p_tty->p_console)) {
		keyboard_read(p_tty);
	}
}


/*======================================================================*
			      tty_do_write
 *======================================================================*/
PRIVATE void tty_do_write(TTY* p_tty)
{
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;
		if(!isSearch)
		out_char(p_tty->p_console, ch);
		else
		out_char_blue(p_tty->p_console, ch);
	}
}


