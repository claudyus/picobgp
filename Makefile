

CFLAGS?=-g
CROSSCOMPILE?=
OBJS=main.o signal.o route.o utils.o

main: $(OBJS)
	$(CROSSCOMPILE)gcc $(CFLAGS) -o pbgp $(OBJS)

.c.o:
	$(CROSSCOMPILE)gcc $(CFLAGS) -c $<

clean:
	rm -fr *.o pbgp

install:
	cp -f pbgp /usr/sbin/pbgp

