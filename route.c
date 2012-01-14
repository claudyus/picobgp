
#include "pbgp.h"

/* see http://www.cs.clemson.edu/~westall/826/zebra-0.92a/zebra/rt_ioctl.c for route table manipolation*/
void clean_rt(void) {
	int i;
	for (i = 0; i < ann_len; i++) {
		if (difftime(time(NULL), ann_ips[i].l_update) > 10) {
			/* network not more reachable */
			DEFandNULL(struct rtentry, rtentry)
			memcpy (&rtentry.rt_dst, &ann_ips[i].addr, sizeof (struct sockaddr_in));
			memcpy (&rtentry.rt_genmask, &ann_ips[i].netmask, sizeof (struct sockaddr_in));

			if(ioctl (sd, SIOCDELRT, &rtentry) == -1) {
				perror("SIOCDELRT");
			}

			annips_del(i);
		}
	}
}


int update_rt (bgpmsg *flood_msg) {

	/*add rules to route table if it is not already here */
	int i;
	for (i = 0; i < ann_len; i++) {
		if (memcmp(&ann_ips[i].addr, &(flood_msg->addr), sizeof(struct in_addr)*2) == 0)
			return 0;	//route already in place
	}

	DEFandNULL(struct rtentry, rtentry)
	memcpy (&rtentry.rt_dst, &(*flood_msg).addr, sizeof (struct sockaddr_in));
	memcpy (&rtentry.rt_genmask, &(*flood_msg).netmask, sizeof (struct sockaddr_in));
	rtentry.rt_flags = RTF_UP | RTF_HOST;

	if(ioctl (sd, SIOCADDRT, &rtentry) == -1) {
		perror("SIOCADDRT");
	}

	annips_add(*flood_msg);
}

