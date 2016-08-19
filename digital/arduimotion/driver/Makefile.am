bin_PROGRAMS = am_driver
am_driver_SOURCES = main.c config.h xlib_bridge.c xlib_bridge.h ansi.h
AM_LDFLAGS = @GLIB_LIBS@
AM_LDFLAGS += -L/usr/local/lib -lX11 -lXtst
AM_CFLAGS = @GLIB_CFLAGS@
# For FreeBSD and other BSD systems. intl.h is installed in a non-standard place...
AM_CFLAGS += -I/usr/local/include
