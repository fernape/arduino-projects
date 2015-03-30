/*
   Copyright (c) 2015, Fernando Apesteguia
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

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
 * Change volume of an LG TV using a SRF05 ultrasonic sensor
 * (single pin configuration)
 */

#include "common.h"
#include "serial.h"
#include "lgprotocol.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

static int echo_pin = PD2;				/* Arduino's digital pin #2 */
static const int echo_length_us = 15; 			/* 10 us pulse width is the minimun length required */
static const int measurments_delay_ms = 20;		/* 10 ms between echoes at least */


/* Set pin as input */
void
pin_as_input()
{
	/* 
	   Set echo_in_pin as input
	 */
	DDRD &= ~(_BV(echo_pin));
}

/* Set pin as output */
void
pin_as_output()
{
	/* 
	   Set echo_out_pin as output.
	 */
	DDRD |= _BV(echo_pin);
}
/*
   Set echo_out_pin position in PORTD to 1.
   Leave the rest of the bits untouched
 */
inline void
pin_set_high()
{
	/* Set second pin in the register */
	/*PGM_P str = "Output high...\n";
	printf(str);
	fflush(stdout);*/
	PORTD |= _BV(echo_pin);
}
/*
   Set echo_out_pin position in PORTD to 0.
   Leave the rest of the bits untouched
 */
inline void
pin_set_low()
{
	/* Clear the register */
	/*PGM_P str = "Output low...\n";
	printf(str);
	fflush(stdout);*/
	PORTD &= ~(_BV(echo_pin));
}

/*
   Waits for the answer of the echo and returns
   the length of the pulse in cm.
 */
int
read_value()
{

	int pulse_length_ms = 0;

	/* Wait until we receive the response */
	loop_until_bit_is_set(PIND, echo_pin);

	while (PIND & _BV(echo_pin)) { 
		_delay_us(1); 
		pulse_length_ms++;
	}

	/* 
	   The length of the pulse is the distance in inches,
	   so divide it by 58 to obtain cm.
	 */
	return (pulse_length_ms / 58);
}

/*
   Sends an echo 
 */
void
send_echo()
{

	/* Ensure we start from low impedance */
	pin_set_low();
	pin_set_high();
	_delay_us(echo_length_us);
	pin_set_low();
}

/*
  Sends an echo an reads the response.
  It returns the distance in cms.
 */
int
measure_distance()
{
	int pulse_length = 0;

	pin_as_output();

	/* Send echo */
	send_echo();

	pin_as_input();

	/* Read the echo response */
	pulse_length = read_value();

	return (pulse_length); /* Divide by 74 to get the result in inches */
}

/*
   Program entry point
 */

int main(int argc, char **argv)
{
	int distance = 0;
	int prev_volume = 5;
	int vol = 0;

	/* Initialize serial port */
	SIO_INIT;

	for (;;) {
		/* Measure distance by sending a pulse and reading the response */
		distance = measure_distance();

		/* Some calibration for my example */
		vol = distance / 2;

		if (vol != prev_volume) {
			lg_set_volume(vol);
		}

		/* Wait at least 10 ms */
		prev_volume = vol;
		_delay_ms(measurments_delay_ms);
	}


	/* Make the compiler happy */
	return (0);
}




