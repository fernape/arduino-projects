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
 * Header file for my implementation fo the protocol for the LG32LH301C TV
 * (and probably other models too).
 */

#ifndef __LG32LH301C_PROTOCOL__
#define __LG32LH301C_PROTOCOL__

/*
 * LG 32LH301C
 * Declare the API for the protocol
 */

enum LGAspectRatios {
	LG_ASPECT_NORMAL = 1,
	LG_ASPECT_PANORAMIC = 2,
	LG_ASPECT_ZOOM = 4,
	LG_ASPECT_ORIGINAL = 6,
	LG_ASPECT_14_9 = 7,
	LG_ASPECT_FULL_WIDTH = 8,
	LG_ASPECT_SCAN_ONLY = 9,
	LG_CINE_ZONE_1 = 0x10,
	LG_CINE_ZONE_2 = 0x11,
	LG_CINE_ZONE_3 = 0x12,
	LG_CINE_ZONE_4 = 0x13,
	LG_CINE_ZONE_5 = 0x14,
	LG_CINE_ZONE_6 = 0x15,
	LG_CINE_ZONE_7 = 0x16,
	LG_CINE_ZONE_8 = 0x17,
	LG_CINE_ZONE_9 = 0x18,
	LG_CINE_ZONE_10 = 0x19,
	LG_CINE_ZONE_11 = 0x1A,
	LG_CINE_ZONE_12 = 0x1B,
	LG_CINE_ZONE_13 = 0x1C,
	LG_CINE_ZONE_14 = 0x1D,
	LG_CINE_ZONE_15 = 0x1E,
	LG_CINE_ZONE_16 = 0x1F
};

enum LGColorTemps { LG_COLORTEMP_FLAT, LG_COLORTEMP_COLD, LG_COLORTEMP_WARM };

enum LGInputs {
	LG_INPUT_DTV = 0,
	LG_INPUT_ANALOG = 16,
	LG_INPUT_AV = 32,
	LG_INPUT_COMPONENT = 64,
	LG_INPUT_RGB = 96,
	LG_INPUT_HDMI = 144
};

enum LGEnergySavings {
	LG_ENERGY_SAVING_OFF = 0,
	LG_ENERGY_SAVING_MINIMUM = 1,
	LG_ENERGY_SAVING_MEDIUM = 2,
	LG_ENERGY_SAVING_MAXIMUM = 3
};

int lg_switch_on(void);                       	/* Turn TV on */
int lg_switch_off(void);                	/* Turn TV off */
int lg_set_aspect_ratio(const char);        	/* Set aspect ratio. Use LGAspectRatios */
int lg_enable_mute_screen(void);
int lg_enable_mute_screen_and_video(void);
int lg_disable_mute_screen(void);
int lg_enable_mute_volume(void);
int lg_disable_mute_volume(void);
int lg_set_volume(const char);
int lg_set_contrast(const char);
int lg_set_brigthness(const char);
int lg_set_color(const char);
int lg_set_matiz(const char);
int lg_set_definition(const char);
int lg_activate_OSD(void);
int lg_deactivate_OSD(void);
int lg_lock_controls(void);                	/* lock both IR commands and front panel buttons */
int lg_unlock_controls(void);
int lg_set_acutes(const char);
int lg_set_bass(const char);
int lg_set_balance(const char);
int lg_set_color_temp(const char);       	/* Use LGColorTemps */
int lg_autoconfig(void);                	/* Autoconfigs TV but _only_ in PC mode */
int lg_set_backlight(const char);        
int lg_select_input(const char);        	/* Use LGInputs */
int lg_set_energy_saving(const char);		/* Use LGEnergySavings */

#endif /* __LG32LH301C_PROTOCOL__ */
