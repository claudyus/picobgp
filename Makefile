

CFLAGS?=-g
CROSSCOMPILE?=
GCC?=$(CROSSCOMPILE)gcc
OBJS=main.o signal.o route.o utils.o

main: $(OBJS)
	$(GCC) $(CFLAGS) -o pbgp $(OBJS)

.c.o:
	$(GCC) $(CFLAGS) -c $<

debian: main
	equivs-build -f distro/debian/picobgp
	make clean

clean:
	rm -fr *.o pbgp

install:
	cp -f pbgp /usr/local/sbin/pbgp

