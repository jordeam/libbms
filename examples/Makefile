GLIB_CFLAGS = $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS = $(shell pkg-config --libs glib-2.0)

LDFLAGS = -lbms -lm $(GLIB_LDFLAGS)
CFLAGS = -I /usr/include/bms $(GLIB_LDFLAGS)
