#INCDIR = ./include
INCLUDES = -I .

GLIB_CFLAGS = $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS = $(shell pkg-config --libs glib-2.0)

CFLAGS = -O2 $(INCLUDES) -Wall -D_GNU_SOURCE $(GLIB_CFLAGS) -fPIC
OBJCFLAGS = -O2 -Wall $(INCLUDES) -D_GNU_SOURCE $(GLIB_CFLAGS)
CC = gcc
AR = ar
RANLIB = ranlib
AR_FLAGS = -cru
LDFLAGS = -lm -lpthread $(GLIB_LDFLAGS)

HEADERFILES = asmline.h bcomplex.h bvector.h CObject.h \
	constant.h \
	fformat.h fourier.h fourier_serie.h func.h integnum.h log.h \
	modulations.h nlsol.h \
	ondas.h ondas2.h polinom.h \
	strbms.h ripple.h runge.h rvector.h cvector.h \
	sysconst.h \
	transfor.h vvector.h wav.h \
	runge51.h
#	serial_raw.h 
#	LnList.h LnElem.h AsmLine.h

OBJCSRCS = $(shell ls *.m)

CSRCS = asmline.c bcomplex.c bvector.c cvector.c CObject.c \
	fformat.c fourier.c fourier_serie.c \
	func.c integnum.c \
	modulations.c nlsol.c \
	ondas.c ondas2.c polinom.c \
	ripple.c runge5.c runge51.c rvector.c strss.c string_split.c \
	transfor.c vvector.c wav.c \
	#	serial_raw.c # LnList.c LnElem.c AsmLine.c

SRCS = $(CSRCS) # $(OBJCSRCS)

OBJS = $(patsubst %.c,%.o,$(CSRCS)) # $(patsubst %.m,%.o,$(OBJCSRCS))


ALL = libbms.so libbms.a

%.o: %.c %.h
	gcc $(CFLAGS) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) -o $@ -c $<

all: all-subdirs $(ALL)

all-subdirs:
#	make -C scheme

teste:
	@echo $(OBJCSRCS)

objs: $(OBJS)
	touch objs

strss.o: strss.c strbms.h
	gcc $(CFLAGS) -D_GNU_SOURCE -c $<

libbms.a: $(OBJS)
	@echo "Making lib..."
	rm -f $@
	$(AR) $(AR_FLAGS) $@ $(OBJS)
	$(RANLIB) $@
	@echo "done."

libbms.so: $(OBJS)
	@echo "Making lib..."
	rm -f $@
	$(CC) -shared -o $@ $^
	@echo "done."

clean:
	rm -f *~ *.o
	make -C scheme clean

distclean: clean
	rm -f $(ALL) a.out
	rm -f *-stamp
	make -C scheme distclean

%.o: %.m %.h
	gcc $(OBJCFLAGS) -o $@ -c $<

%.o: %.m
	gcc $(OBJCFLAGS) -o $@ -c $^

dirs:
	mkdir -p $(DESTDIR)/usr/lib
	mkdir -p $(DESTDIR)/usr/include/bms


install: dirs libbms.so libbms.a
# header files
	for n in $(HEADERFILES); do \
	  install -o root -g root -m 0644 $$n $(DESTDIR)/usr/include/bms; \
	done
	install -o root -g root -m 0755 libbms.so $(DESTDIR)/usr/lib/
	install -o root -g root -m 0755 libbms.a $(DESTDIR)/usr/lib/

# string_split: string_split.c strss.o
# 	$(CC) $(CFLAGS) $^ -lglib -D TEST -I /usr/include/glib-1.2/ -I /usr/lib/glib/include/ -lglib -o $@
