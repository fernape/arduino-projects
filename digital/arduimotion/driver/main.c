/*
   Arduimotion user space driver.
   Ape, Sun Jan 30 19:53:37 CET 2011
 */

#include <glib.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "xlib_bridge.h"


/*
   Callback to be invoked when new data arrives to the file
 */
gboolean read_data(GIOChannel *source, GIOCondition condition, gpointer data)
{
	GIOStatus status;
	gsize terminator_pos;
	GString *buffer;
	GError *error = NULL;
	int x, y, button_pressed;
	int ret_val;

	buffer = g_string_new("");

	status = g_io_channel_read_line_string(source,
			buffer,
			&terminator_pos,
			&error);

	if((status == G_IO_STATUS_ERROR) || (status == G_IO_STATUS_AGAIN)) {
		g_warning("Problem reading from channel: %s", error->message);
		return TRUE;
	}


	if ((ret_val = sscanf(buffer->str, "%d,%d,%d\n", &x, &y, &button_pressed)) == 0) {
		g_warning("Can't parse data. I received: %s", buffer->str);
	} else {
		update_mouse_state(x, y, button_pressed);
	}

	/* Free resources */
	g_string_free(buffer, TRUE);

	return TRUE;
}

/* Show help information */
void show_usage(char *me)
{
	g_print("%s %s\n", PACKAGE_NAME, VERSION);
	g_print("Usage: %s <device_file>\n", me);
}

/* Program entry point */
int main(int argc, char **argv)
{
	GMainLoop *event_loop;
	GIOChannel *read_channel;
	GError *error = NULL;

	/* Take parameter (device file) */
	if(argc !=2) {
		show_usage(argv[0]);
		return 0;
	}

	/* Init xlib bridge */
	init_bridge();

	/* Create IO Channel */
	read_channel = g_io_channel_new_file(argv[1], "r", &error);
	/* We just want to read raw data */
	g_io_channel_set_encoding(read_channel, NULL, NULL);

	if(read_channel == NULL) {
		g_critical("Error while creating IO Channel: %s", error->message);
		return 0;
	}

	/* Create event loop */
	event_loop = g_main_loop_new(NULL, FALSE);

	/* Add callback to event loop */
	g_io_add_watch(read_channel, G_IO_IN, read_data, NULL);

	/* Loop until g_main_loop_quit() */
	g_main_loop_run(event_loop);

	/* Close Xlib bridge */
	shutdown_bridge();

	/* Back to system */
	return 0;
}
