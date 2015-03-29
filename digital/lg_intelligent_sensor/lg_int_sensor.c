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
 * Reads the value of a photoresistor and sets the
 * power saving settings of an LG TV through the
 * serial port.
 *
 * With 13K resistors for the photoresistor circuit
 * we have a range from [~0 - ~700].
 * Ape, Thu Sep 23 19:22:52 CEST 2010
 */

#include "common.h"
#include "lgprotocol.h"
#include "serial.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>



void
adc_init(void)
{
	/* Disable pull up resistors. They interfer with the sampling */
	PORTC = 0;
	DDRC = 0;

	/* Use Gnd as reference and connect ADC2 to the AD Converter */
	ADMUX = _BV(REFS0) | 0x02;

	/* Now divide the clock frequency by a factor of 
	   128 (ADPS[0-2]) set to 1.
	   Enable the converter (ADEN)
	   and do a first sample (ADSC). This last step is to force all the
	   internal setup that takes place for the first sample.
	 */

	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADSC); 
	/* Wait for the read to complete */
	loop_until_bit_is_clear(ADCSRA, ADSC);

}


int
adc_read(unsigned char pin)
{
	unsigned char low, high, channel;

	channel = (ADMUX & 0xf0) | (pin & 0x0f);

	/* Select the channel */
	ADMUX = channel;

	/* Read the value... */
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);

	low = ADCL;
	high = ADCH;

	/* Return value composition */
	return (high << 8) | low;


}

void
set_state(const int state, int *current)
{

	lg_set_energy_saving(state);
	*current = state;
}

/* Program entry point */
int main(int argc, char **argv)
{
	int current_state = LG_ENERGY_SAVING_OFF;
	const int sampling_delay = 2 * 1000; /* 2 seconds */

	/* set up stdio */
	SIO_INIT;

	/* ADC converter initialization */
	adc_init();


	for (;;) {
		int photo_value= adc_read(2);

		if (photo_value >= 450 && current_state != LG_ENERGY_SAVING_MAXIMUM) {
			/* Too dark, maximum energy save */
			set_state(LG_ENERGY_SAVING_MAXIMUM, &current_state);	
		} else if (photo_value < 400 && photo_value >= 300 
				&& current_state != LG_ENERGY_SAVING_MEDIUM) {
			/* Medium energy saving */
			set_state(LG_ENERGY_SAVING_MEDIUM, &current_state);
		} else if (photo_value < 300 && photo_value > 150 
				&& current_state != LG_ENERGY_SAVING_MINIMUM) {
			/* Minimum energy saving */
			set_state(LG_ENERGY_SAVING_MINIMUM, &current_state);
		} else if (photo_value < 150 && current_state != LG_ENERGY_SAVING_OFF) {
			/* Too bright, deactivate energy saving */
			set_state(LG_ENERGY_SAVING_OFF, &current_state);
		}

		_delay_ms(sampling_delay);
	}

	return 0;
}
