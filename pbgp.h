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

#define DEFandNULL(type, name) type name; memset(&name, 0, sizeof(type));

typedef struct sbgpmsg {
	struct in_addr addr;
	struct in_addr netmask;
	struct timeval l_update;
} bgpmsg;

int sd;		//udp socket descriptor
int rd;		//route sock descriptor

int ipslen;
bgpmsg *ips;

int ann_len;
bgpmsg *ann_ips;


void signal_handler (int type);
void clean_rt(void);
void update_rtfrom (void);

