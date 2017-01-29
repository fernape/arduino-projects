/*
   Copyright (c) 2017, Fernando Apesteguia
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
 * Program to make bubbles when the sensor reads out some values
 * higher than a certain threshold.
 */

#include "common.h"
#include "serial.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>


/* LED pins */
#define GREEN_LED PD2
#define YELLOW_LED PD3
#define RED_LED PD4

/* Thresholds for the different states */
#define GREEN_LIMIT 60
#define YELLOW_LIMIT 80

/* How many values we rely on to calculate the average */
#define AVG_VALUES 10

/* Global variable */
static int read_values_array[AVG_VALUES];

/*
 * A/D converter initialization
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
 * Performs a read from the passed pin
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
 * Set which pins are out for the leds
 */
void
setup_led_pins()
{
	/* Data Direction Register for port D */
	DDRD = 0xFF;
	DDRD |= _BV(RED_LED);
	DDRD |= _BV(YELLOW_LED);
	DDRD |= _BV(GREEN_LED);
}

/*
 * Switch led passed as parameter on
 */
void
led_on(const int led)
{
	PGM_P str = "led on: %d\n";
	PORTD = _BV(led);
	printf(str, led);
}

/*
 * Move the stick up so it moves closer
 * to the fan
 */
void
swing_stick_up()
{
	/*
	 * Magic value to move the stick up to a certain
	 * point
	 */
	OCR1A = 510;
}

/*
 * Move the stick down so it moves away
 * from the fan
 */
void
swing_stick_down()
{
	/*
	 * Magic value to move the stick down to a certain
	 * point
	 */
	OCR1A = 130;
}

/*
   * For a very good explanation on how to set up
   * counters and PWM for controlling servo motors
   * see:
   * 
   * http://extremeelectronics.co.in/avr-tutorials/servo-motor-control-by-using-avr-atmega32-microcontroller/
   * http://www.embedds.com/controlling-servo-motor-with-avr/
   */
void
setup_pwm_timers()
{

	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

	ICR1 = 4999;  //fPWM=50Hz (Period = 20ms Standard).

	DDRB |= (1<<PB1);   //PWM Pins as Out

}

/*
 * Start fan
 */
void
fan_on()
{

	DDRB |= _BV(PB4);
	PORTB = _BV(PB4);	
}

/*
   * Stop fan
 */
void
fan_off()
{

	PORTB &= ~_BV(PB4);
}

/*
 * There's too much noise. Let's make some bubbles!
 */
void
warn_too_loud()
{
	swing_stick_up();
	_delay_ms(500);
	fan_on();
	_delay_ms(1500);
	swing_stick_down();
	fan_off();
	_delay_ms(1000);
}

/*
 * Check and change our state depending on sensor read out.
 */
void
check_led_lights(const int sensor_value)
{
	if (sensor_value < GREEN_LIMIT) {
		led_on(GREEN_LED);
	} else  if (sensor_value > GREEN_LIMIT && sensor_value < YELLOW_LIMIT) {
		led_on(YELLOW_LED);
	} else if (sensor_value > YELLOW_LIMIT) {
		led_on(RED_LED);
		warn_too_loud();
	}
}

/*
 * Init array where we store the sensor read outs
 */
void
init_read_values_array()
{
	memset(read_values_array, 0, sizeof(read_values_array));
}

/*
 * Calculate average of stored readouts
 */
int
calculate_avg(const int read_value)
{
	static int position = 0;
	int i, sum = 0;

	read_values_array[position % AVG_VALUES] = read_value;

	if (++position == AVG_VALUES) {
		position = 0;
	}
	
	for (i = 0; i < AVG_VALUES; i++) {
		sum += read_values_array[i];
	}

	return (sum / AVG_VALUES);
}


/* Program entry point */
int main(int argc, char **argv)
{
	int read_value;
	int avg;
	const unsigned int adc_read_delay = 50; /* In ms. */
	PGM_P read_str = "value = %d\n";

	/* First things first... */
	adc_init();

	/* Serial port initialization */
	SIO_INIT;

	/* Setup pins for LEDs as outputs */
	setup_led_pins();

	/* Same for PWM */
	setup_pwm_timers();

	/* Move stick into the bucket */
	swing_stick_down();

	/* Clear read outs storage */
	init_read_values_array();

	/* Forever loop */
	for (;;) {
		read_value = adc_read(2);
		avg = calculate_avg(read_value);
		printf(read_str, avg);
		check_led_lights(avg);
		_delay_ms(adc_read_delay);
	}

	return 0;
}
