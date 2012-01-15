#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> //malloc
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>


#define DEFandNULL(type, name) type name; memset(&name, 0, sizeof(type));

typedef struct sbgpmsg {
	struct in_addr addr;
	struct in_addr netmask;
	struct in_addr loc_addr;
	time_t l_update;
} bgpmsg;

int sd;		//udp socket descriptor

extern int ips_len;
extern bgpmsg *ips;

extern int ann_len;
extern bgpmsg *ann_ips;

char local_ifname[IFNAMSIZ];
struct in_addr loc_ip, brd_ip;

//signal.c
void signal_handler (int type);
//route.c
void clean_rt(void);
int update_rt (bgpmsg *flood_msg);
//utils.c
int ntomask (int n);
void annips_add(bgpmsg info);
void annips_del(int id);
