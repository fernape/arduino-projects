/*
   Simple connection functions to talk to Xlib.
   It just implements some functions to move the mouse pointer
   and to generate pressed and released button events.

   Ape, Sun Jan 23 21:17:07 CET 2011
 */

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "xlib_bridge.h"


static Display *display = NULL;
static Window root;
enum mouse_event  { MOUSE_BUTTON_RELEASED, MOUSE_BUTTON_PRESSED };
static int screen_width, screen_height;
const static int accel_max = 700;
const static int accel_min = 400;
static int accel_mean;
const static int num_steps = 10;

#define LAST_X_INIT 350
#define LAST_Y_INIT 350


/*
 * Initialization function.
 * First thing to do in order to use the library
 */

int
init_bridge(void)
{

	/* Connect to the default screen */

	if ((display = XOpenDisplay(0)) == NULL) {
		perror("Can't connect to display");
		return (-1);
	}

	root = DefaultRootWindow(display);

	screen_height = DisplayHeight(display, 0);
	screen_width = DisplayWidth(display, 0);
	accel_mean = (accel_min / 2) + (accel_max / 2);

	fprintf(stderr, "Display dimmensions: %dx%d\n", screen_width, screen_height);

	return (0);
}


/*
 * Shutdown function.
 * Last thing to do before ending using
 * the library
 */

void
shutdown_bridge(void)
{

	if (!display)
		return;

	XCloseDisplay(display);
}

int
update_mouse_state(const int accel_x, const int accel_y, const int button_status)
{
	static int prev_button_state = -1;

	if (move_mouse_cursor(accel_x, accel_y) < 0) {
		return (-1);
	}

	if (button_status != prev_button_state) {
		if (button_status == MOUSE_BUTTON_PRESSED) {
			return (mouse_button_pressed());
		} else {
			return (mouse_button_released());
		}
		prev_button_state = button_status;
	}
		
}

int
move_mouse_cursor(const int accel_x, const int accel_y)
{
	int x, y;

	if (!display)
		return (-1);


	x = calculate_x_position(accel_x);
	y = calculate_y_position(accel_y);

	fprintf(stderr, "x = %d\ty = %d\n", x, y);
	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	XFlush(display);

	return (0);
}


/*
 * This function calculates the position of the x
 * coordinate of the mouse pointer. Be aware of
 * the two comparisons below: they are calculated
 * this way because the accelerometer reaches the
 * maximum and minimun values in a non-intuitive
 * position.
 */

int calculate_x_position(const int accel_x)
{
	static int last_x = LAST_X_INIT;
	int increment;

	fprintf(stderr, "accel_x = %d\n", accel_x);
	if (accel_x > accel_mean) {
		increment = ((accel_x - accel_mean) / num_steps) - 1;
		if (!increment || increment == 1)
			return (last_x);

		/* See function documentation!! */
		last_x -= increment;
		fprintf(stderr, "increment_x = %d\n", increment);
	}


	if (accel_x < accel_mean) {
		increment =  (((accel_mean - accel_x) / num_steps) - 1);
		if (!increment || increment == 1)
			return (last_x);

		
		/* See function documentation!! */
		last_x += increment;
		fprintf(stderr, "increment_x = %d\n", increment);
	}

	if (last_x > screen_width)
		last_x = screen_width;
	if (last_x < 0)
		last_x = 0;

	return (last_x);
}


int calculate_y_position(const int accel_y)
{
	static int last_y = LAST_Y_INIT;
	int increment;

	fprintf(stderr, "accel_y = %d\n", accel_y);
	if (accel_y > accel_mean) {
		increment = ((accel_y - accel_mean) / num_steps) - 1;
		if (!increment || increment == 1)
			return (last_y);

		last_y += increment;
		fprintf(stderr, "increment_y = %d\n", increment);
	}


	if (accel_y < accel_mean) {
		increment =  (((accel_mean - accel_y) / num_steps) - 1);
		if (!increment || increment == 1)
			return (last_y);

		last_y -= increment;
		fprintf(stderr, "increment_y = %d\n", increment);
	}

	if (last_y > screen_height)
		last_y = screen_height;
	if (last_y < 0)
		last_y = 0;

	return (last_y);
}

static
int mouse_button_event(int mouse_event)
{
	XEvent event;
	long mask;

	memset(&event, 0, sizeof(event));

	event.xbutton.subwindow = DefaultRootWindow (display);
        event.xbutton.button = Button1; /* Defined in X.h */

	while (event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;
		XQueryPointer (display, event.xbutton.window,
				&event.xbutton.root, &event.xbutton.subwindow,
				&event.xbutton.x_root, &event.xbutton.y_root,
				&event.xbutton.x, &event.xbutton.y,
				&event.xbutton.state);
	}

	if (mouse_event == MOUSE_BUTTON_PRESSED) {
	//	event.type = ButtonPress;
	//	mask = ButtonPressMask;
		XTestFakeButtonEvent(display, Button1, True, CurrentTime);
	} else {
		XTestFakeButtonEvent(display, Button1, False, CurrentTime);
	//	event.type = ButtonRelease;
	//	mask = ButtonReleaseMask;
	}
        event.xbutton.same_screen = True;
	
	//if (XSendEvent(display, PointerWindow, True, mask, &event) == 0)
        //        fprintf(stderr, "Error while sending event\n");

	XFlush(display);


}

int
mouse_button_pressed(void)
{

	fprintf(stderr, "Mouse pressed\n");
	return (mouse_button_event(MOUSE_BUTTON_PRESSED));
}



int
mouse_button_released(void)
{

	fprintf(stderr, "Mouse released\n");
	return (mouse_button_event(MOUSE_BUTTON_RELEASED));
}
