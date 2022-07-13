PREFIX ?= /usr/local

REQ = components/utils
COM =\
	 components/mpdstatus\
	 components/cpu_usage\
	 components/battery\
	 components/nettraf

output: dwmblocks.c blocks.h
	cc `pkg-config --cflags x11` `pkg-config --libs x11` -lmpdclient -g dwmblocks.c $(COM:=.c) $(REQ:=.c) -o dwmblocks
clean:
	rm -f *.o *.gch dwmblocks
install: output
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f dwmblocks $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dwmblocks
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwmblocks
