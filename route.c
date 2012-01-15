
#include "pbgp.h"

/* see http://www.cs.clemson.edu/~westall/826/zebra-0.92a/zebra/rt_ioctl.c for route table manipolation*/
void clean_rt(void) {
	int i;
	for (i = 1; i <= ann_len; i++) {
		if (difftime(time(NULL), ann_ips[i].l_update) > 10) {
			/* network not more reachable */
			DEFandNULL(struct rtentry, rtentry)
			((struct sockaddr_in *)&rtentry.rt_dst)->sin_family = AF_INET;
			((struct sockaddr_in *)&rtentry.rt_gateway)->sin_family = AF_INET;
			((struct sockaddr_in *)&rtentry.rt_genmask)->sin_family = AF_INET;
			((struct sockaddr_in *)&rtentry.rt_dst)->sin_addr.s_addr = ann_ips[i].addr.s_addr;
			((struct sockaddr_in *)&rtentry.rt_genmask)->sin_addr.s_addr = ann_ips[i].netmask.s_addr;
			((struct sockaddr_in *)&rtentry.rt_gateway)->sin_addr.s_addr = ann_ips[i].loc_addr.s_addr;

			if(ioctl (sd, SIOCDELRT, &rtentry) == -1) {
				perror("SIOCDELRT");
			}

			annips_del(i);
		}
	}
}


int update_rt (bgpmsg flood_msg, int len) {

	/*add rules to route table if it is not already here */
	int i;
	for (i = 1; i <= ann_len; i++) {
		if (memcmp(&ann_ips[i].addr, &(flood_msg.addr), sizeof(struct in_addr)*3) == 0)
			return 0;	//route already in place
	}

	DEFandNULL(struct rtentry, rtentry);
	((struct sockaddr_in *)&rtentry.rt_dst)->sin_family = AF_INET;
	((struct sockaddr_in *)&rtentry.rt_gateway)->sin_family = AF_INET;
	((struct sockaddr_in *)&rtentry.rt_genmask)->sin_family = AF_INET;

	((struct sockaddr_in *)&rtentry.rt_dst)->sin_addr.s_addr = flood_msg.addr.s_addr;
	((struct sockaddr_in *)&rtentry.rt_genmask)->sin_addr.s_addr = flood_msg.netmask.s_addr;
	((struct sockaddr_in *)&rtentry.rt_gateway)->sin_addr.s_addr = flood_msg.loc_addr.s_addr;
	//ftp://ftp.gwdg.de/linux/funet/PEOPLE/.Linus.old/route.c
	//strcpy(&rtentry.rt_if, local_ifname);
	rtentry.rt_flags = RTF_UP | RTF_GATEWAY;
	rtentry.rt_metric = 2;
	rtentry.rt_mtu = 1;


	fprintf(stderr, "recieved %s netmask %s\n", inet_ntoa(flood_msg.addr), inet_ntoa(flood_msg.netmask));

	if(ioctl (sd, SIOCADDRT, &rtentry) == -1) {
		perror("SIOCADDRT");
	}

	annips_add(flood_msg);
}

