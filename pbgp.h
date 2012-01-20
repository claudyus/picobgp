#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
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

int sd;		/* udp socket descriptor */

extern int ips_len;
extern bgpmsg *ips;

extern int ann_len;
extern bgpmsg *ann_ips;

char local_ifname[IFNAMSIZ];
struct in_addr loc_ip, brd_ip;
extern int cascade;
extern int one_shot;

/* function in signal.c */
void let_me_spam(void);
void signal_handler (int type);
/* function in route.c */
void clean_rt(int force);
int update_rt (bgpmsg flood_msg, int len);
/* function utils.c */
void annips_add(bgpmsg info);
void annips_del(int id);
void parse_opt(int argc, char *argv[]);
void get_if_info(void);
int add_ips(char *cidr);
