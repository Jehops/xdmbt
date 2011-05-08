CFLAGS= $(shell xft-config --cflags) $(SLKCFLAGS)
LDFLAGS= $(SLKCFLAGS) $(shell xft-config --libs) -lXpm
PROG= xdmbt
XPM= reboot_in.xpm reboot_out.xpm shutdown_in.xpm shutdown_out.xpm

$(PROG): xdmbt_main.o xdmbt_util.o xdmbt_button.o
	$(LINK.C) -o $@ xdmbt_main.o xdmbt_util.o xdmbt_button.o

xdmbt_main.o: xdmbt_main.c xdmbt.h $(XPM)
	$(COMPILE.c) $*.c -o $@

xdmbt_util.o: xdmbt_util.c xdmbt.h
	$(COMPILE.c) $*.c -o $@

xdmbt_button.o: xdmbt_button.c xdmbt.h
	$(COMPILE.c) $*.c -o $@

clean:
	rm -f *.o $(PROG)

debug: CFLAGS += -O0 -DDEBUG -ggdb -Wall
debug: $(PROG)
