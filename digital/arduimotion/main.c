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
   Create a device similar to wiimotion using a button and a 
   2-axis acceleremeter. Needs the user space 'driver' to talk
   to the X server through the Xlib to send the mouse events.

	   * Click button is conected to digital pin #2
	   
	   * Activation button is conected to digital pin #3

	   * Acceleremeter is connected as follows:
		X axis: analog input #2
		Y axis: analog input #3


   Ape, martes, 16 de agosto de 2016, 17:14:58 CEST
 */


#include "common.h"
#include "serial.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define LEDPORT	PORTB
#define LEDPIN	0x20


/*
   Set the input pin for the button
   and the output pin for the LED
 */

void
button_setup()
{

	/* Now for the input pin (pin #2 at register D) */
	DDRD = 0x00;
	PORTD = 0X00;
}


/*
   Do the AD Converter initialization.
   It includes a first read operation which results are discarded
 */

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


/*
   Do an AD conversion and read the value from 'pin'
 */

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

/*
 * Debounce bouton reads
 */
int
debounce(int pin, int mask)
{
	static const int MAX_LOOPS = 50;

	int cont = 0;
	int i;

	for (i = 0; i < MAX_LOOPS; i++) {
		if (pin & (1 << mask)) {
			cont++;
		} else {
			cont--;
		}
	}

	return cont >= 0;
}


/* Program entry point */
int main(int argc, char **argv)
{
	int x_axis_value, y_axis_value, button_status;
	PGM_P values_str = "%d,%d,%d\n";

	/* First things first... ADC setup */
	adc_init();

	/* Button pin setup */
	button_setup();

	/* Serial port initialization */
	SIO_INIT;

	for (;;) {
		/* Read accelerometer values */
		x_axis_value = adc_read(2);
		y_axis_value = adc_read(3);

		/* Read button status */
		button_status = debounce(PIND, PD2);

		printf(values_str, x_axis_value, y_axis_value, button_status);
	}

	return 0;
}

