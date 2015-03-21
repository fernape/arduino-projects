/*
   Copyright (c) 2015, Fernando Apesteguia
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this 
   list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
   POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * LG32LH301C protocol implementation
 * All values passed to these functions should be 
 * expressed as hexadecimal, i.e a value of 64
 * for 'volume' is a 100 decimal (the maximum)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lgprotocol.h"


#define ID        1                	/* Target TV ID */
#define CMD_FMT "%c%c %d %d\n" 		/* [Command1][Command2][][SetID][][Data][CR] */
#define ACK_FMT "%c %d OK %d\n"
#define ERR_FMT "%c %d NG %d\n"
#define BUF_SZ 8

#ifdef PC_DEBUG
#define LG_DEBUG(msg)        printf("%s\n", msg);
#else
#define LG_DEBUG(msg)
#endif

char cmd_buff[BUF_SZ];	/* Store the command */

/* Command composition */
int
_lg_create_command(const char cmd1, const char cmd2, const char data)
{
	/* Clear buffer */
	memset(cmd_buff, 0, BUF_SZ);

	/* Build the command string */
	if (sprintf(cmd_buff, CMD_FMT, cmd1, cmd2, ID, data) < 0) {
		/* Some error occurred */
		return -1;
	}

	return 0;
}

void
_lg_send_command()
{
	LG_DEBUG(cmd_buff);
	printf("%s", cmd_buff);
}

void
_lg_tv_send_command(const char cmd1, const char cmd2, const char data)
{
	_lg_create_command(cmd1, cmd2, data);
	_lg_send_command();
}



int
lg_switch_on(void)
{
	_lg_tv_send_command('k', 'a', 1);
	return 0;
}                        

int
lg_switch_off(void)
{
	_lg_tv_send_command('k', 'a', 0);
	return 0;
}                

int
lg_set_aspect_ratio(const char aspect)
{
	_lg_tv_send_command('k', 'c', aspect);
	return 0;
}        

int
lg_enable_mute_screen(void)
{
	_lg_tv_send_command('k', 'd', 0x01);
	return 0;
}

int
lg_enable_mute_screen_and_video(void)
{
	_lg_tv_send_command('k', 'd', 0x10);
	return 0;
}

int
lg_disable_mute_screen(void)
{
	_lg_tv_send_command('k', 'd', 0);
	return 0;
}

int
lg_enable_mute_volume(void)
{
	_lg_tv_send_command('k', 'e', 0);
	return 0;
}

int
lg_disable_mute_volume(void)
{
	_lg_tv_send_command('k', 'e', 1);
	return 0;
}

int
lg_set_volume(const char volume)
{
	_lg_tv_send_command('k', 'f', volume);
	return 0;
}

int
lg_set_contrast(const char contrast)
{
	_lg_tv_send_command('k', 'g', contrast);
	return 0;
}

int
lg_set_brigthness(const char brigthness)
{
	_lg_tv_send_command('k', 'h', brigthness);
	return 0;
}

int
lg_set_color(const char color)
{
	_lg_tv_send_command('k', 'i', color);
	return 0;
}

int
lg_set_matiz(const char matiz)
{
	_lg_tv_send_command('k', 'j', matiz);
	return 0;
}

int
lg_set_definition(const char definition)
{
	_lg_tv_send_command('k', 'k', definition);
	return 0;
}

int
lg_activate_OSD(void)
{
	_lg_tv_send_command('k', 'l', 1);
	return 0;
}
int
lg_deactivate_OSD(void)
{
	_lg_tv_send_command('k', 'l', 0);
	return 0;
}

int
lg_lock_controls(void)
{
	_lg_tv_send_command('k', 'm', 1);
	return 0;
}                

int
lg_unlock_controls(void)
{
	_lg_tv_send_command('k', 'm', 0);
	return 0;
}

int
lg_set_acutes(const char agudos)
{
	_lg_tv_send_command('k', 'r', agudos);
	return 0;
}

int
lg_set_bass(const char bass)
{
	_lg_tv_send_command('k', 's', bass);
	return 0;
}

int
lg_set_balance(const char balance)
{
	_lg_tv_send_command('k', 't', balance);
	return 0;
}

int
lg_set_color_temp(const char color_temp)
{
	_lg_tv_send_command('k', 'u', color_temp);
	return 0;
}        

int
lg_autoconfig(void)
{
	_lg_tv_send_command('j', 'u', 1);
	return 0;
}                

int
lg_set_backlight(const char backlight)
{
	_lg_tv_send_command('m', 'g', backlight);
	return 0;
}        

int
lg_select_input(const char input)
{
	_lg_tv_send_command('x', 'b', input);
	return 0;
}         

int
lg_set_energy_saving(const char saving_level)
{
	_lg_tv_send_command('j', 'q', saving_level);
	return 0;
}
