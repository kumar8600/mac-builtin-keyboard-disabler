CC	= clang
CFLAGS	= -O2 -Wall
DEST	= /usr/local/bin
LIBS	= -lusb-1.0
OBJS	= usbattwatch.o
PROGRAM	= usbattwatch

DISABLERSH = keyboard-disabler.sh
SERVICECONF = net.kumar.keyboard-disabler.plist
SERVICESDEST = /Library/LaunchDaemons


all:	$(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
	rm -f *.o *~ $(PROGRAM)

install:	$(PROGRAM)
	install -s $(PROGRAM) $(DEST)
	install $(DISABLERSH) $(DEST)
	install $(SERVICECONF) $(SERVICESDEST)
	sudo launchctl load $(SERVICESDEST)/$(SERVICECONF)

uninstall:
	sudo launchctl unload $(SERVICESDEST)/$(SERVICECONF)
	rm -f $(SERVICESDEST)/$(SERVICECONF)
	rm -f $(DEST)/$(DISABLERSH)
	rm -f $(DEST)/$(PROGRAM)
