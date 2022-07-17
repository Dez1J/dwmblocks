PREFIX ?= /usr/local

REQ = components/utils
COM =\
	 components/mpdstatus\
	 components/cpu_usage\
	 components/battery\
	 components/nettraf

CC = gcc

# Flags for Clang or Gcc
CFLAGS = -Ofast -g
#CFLAGS = -O0 -g -Wall -Wextra

output: dwmblocks.c blocks.h
	$(CC) $(CFLAGS) `pkg-config --cflags x11` `pkg-config --libs x11` -lmpdclient $(COM:=.c) $(REQ:=.c) dwmblocks.c -o dwmblocks
clean:
	rm -f *.o *.gch dwmblocks
install: output
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f dwmblocks $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dwmblocks
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwmblocks
