#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> //malloc
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#define DEFandNULL(a, b) (a b; memset(&b, 0, sizeof(a))

typedef struct sbgpmsg {
	struct in_addr addr;
	struct in_addr netmask;
	struct timeval l_update;
} bgpmsg;

int sd;
int ipslen;
bgpmsg *ips;


