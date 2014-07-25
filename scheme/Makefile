#INCDIR = ./include
INCLUDES = -I ..

GLIB_CFLAGS = $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS = $(shell pkg-config --libs glib-2.0)

CFLAGS = -O2 $(INCLUDES) -Wall -D_GNU_SOURCE $(GLIB_CFLAGS)
OBJCFLAGS = -O2 -Wall $(INCLUDES) -D_GNU_SOURCE $(GLIB_CFLAGS)
CC = gcc
AR = ar
RANLIB = ranlib
AR_FLAGS = -cru
LDFLAGS = -lm -lobjc -lpthread $(GLIB_LDFLAGS)

SRCS = $(CSRCS) $(OBJCSRCS)

# OBJS = $(patsubst %.c,%.o,$(CSRCS)) $(patsubst %.m,%.o,$(OBJCSRCS))

SCM_OBJS = ../ondas.o scm_ondas.o

ALL = libbms-scm.so

all: $(ALL)

libbms-scm.so: $(SCM_OBJS)
	rm -f $@
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -f *~ *.o

distclean: clean
	rm -f $(ALL)

dirs:
	mkdir -p $(DESTDIR)/usr/lib

install: dirs libbms-scm.so
	install -o root -g root -m 0755 libbms-scm.so $(DESTDIR)/usr/lib/
