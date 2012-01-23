

CFLAGS?=-g
CROSSCOMPILE?=
GCC?=$(CROSSCOMPILE)gcc
OBJS=main.o signal.o route.o utils.o

main: $(OBJS)
	$(GCC) $(CFLAGS) -o pbgp $(OBJS)

.c.o:
	$(GCC) $(CFLAGS) -c $<

clean:
	rm -fr *.o pbgp

install:
	cp -f pbgp /usr/sbin/pbgp

