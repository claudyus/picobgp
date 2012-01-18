/* picobgp - tiny routing advertising daemon
 *
 * Copyright 2012 - Claudio Mignanti <c.mignanti@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License. */

#include "pbgp.h"

void clean_rt(int force) {
	int i;
	for (i = 1; i <= ann_len; i++) {
		if (difftime(time(NULL), ann_ips[i].l_update) > 10 || force) {
			/* network not more reachable */
			DEFandNULL(struct rtentry, rtentry)
			((struct sockaddr_in *)&rtentry.rt_dst)->sin_family = \
			((struct sockaddr_in *)&rtentry.rt_gateway)->sin_family = \
			((struct sockaddr_in *)&rtentry.rt_genmask)->sin_family = AF_INET;
			((struct sockaddr_in *)&rtentry.rt_dst)->sin_addr.s_addr = ann_ips[i].addr.s_addr;
			((struct sockaddr_in *)&rtentry.rt_genmask)->sin_addr.s_addr = ann_ips[i].netmask.s_addr;
			((struct sockaddr_in *)&rtentry.rt_gateway)->sin_addr.s_addr = 0;
			rtentry.rt_dev = local_ifname;

			if(ioctl (sd, SIOCDELRT, &rtentry) == -1) {
				perror("ioctl(SIOCDELRT)");
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
			printf ("update_rt: announce already in ann_ips\n");
			time(&ann_ips[i].l_update);
			return 0;	/* route already in place */
	}

	DEFandNULL(struct rtentry, rtentry);
	((struct sockaddr_in *)&rtentry.rt_dst)->sin_family = \
	((struct sockaddr_in *)&rtentry.rt_gateway)->sin_family = \
	((struct sockaddr_in *)&rtentry.rt_genmask)->sin_family = AF_INET;

	((struct sockaddr_in *)&rtentry.rt_dst)->sin_addr.s_addr = flood_msg.addr.s_addr;
	((struct sockaddr_in *)&rtentry.rt_gateway)->sin_addr.s_addr = 0;
	((struct sockaddr_in *)&rtentry.rt_genmask)->sin_addr.s_addr = flood_msg.netmask.s_addr;

	rtentry.rt_dev = local_ifname;
	rtentry.rt_flags = RTF_UP;

	//fprintf(stderr, "received %s netmask %d, localaddr: %d\n", inet_ntoa(flood_msg.addr), flood_msg.netmask, flood_msg.loc_addr);

	if(ioctl (sd, SIOCADDRT, &rtentry) == -1) {
		perror("ioctl(SIOCADDRT) are you root?");
	}

	annips_add(flood_msg);
}

