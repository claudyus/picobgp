/* picobgp - tiny routing advertising daemon
 *
 * Copyright 2012 - Claudio Mignanti <c.mignanti@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License. */

#include "pbgp.h"

void annips_add(bgpmsg info){
	time(&info.l_update);
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(ann_len+1) );
	memcpy(&ann_ips[ann_len], &info, sizeof(bgpmsg));
	ann_len ++;
}

void annips_del(int id){
	memcpy(&ann_ips[ann_len-1], &ann_ips[id], sizeof(bgpmsg));
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(ann_len-1) );
	ann_len --;
}

void get_if_info(void) {
	struct ifreq ifr;
	/*retrieve address from device */
	strncpy(ifr.ifr_name, local_ifname, IFNAMSIZ-1);
	if (ioctl(sd, SIOCGIFADDR, &ifr) == -1) {
		perror("ioctl(SIOCGIFADDR)");
		exit(1);
	}
	loc_ip.s_addr = (*(struct sockaddr_in *)&ifr.ifr_addr).sin_addr.s_addr;
	ioctl(sd, SIOCGIFBRDADDR, &ifr);
	brd_ip.s_addr = (*(struct sockaddr_in *)&ifr.ifr_broadaddr).sin_addr.s_addr;
}

int add_ips(char *cidr) {
	char *slash = index(cidr, '/');
	*slash = '\0';

	bgpmsg n_ips;
	n_ips.netmask.s_addr =htonl(0xffffffff << (32-atoi(slash+1)));
	inet_aton(cidr, &(n_ips.addr));
	if (loc_ip.s_addr == 0)
		get_if_info();
	n_ips.loc_addr.s_addr = loc_ip.s_addr;

	if (n_ips.addr.s_addr & ~n_ips.netmask.s_addr) {
		printf ("skip the subnet %s/%s conflicts!\n", cidr, slash+1);
		return 0;
	}

	ips = realloc(ips, sizeof(bgpmsg)*(ips_len+1) );
	memcpy(&ips[ips_len], &n_ips, sizeof(bgpmsg));
	ips_len++;
	fprintf(stderr, "add ip to ips %s\n", inet_ntoa(n_ips.addr));
}

void parse_opt(int argc, char *argv[]) {

	int opt;
	while ((opt = getopt(argc, argv, "i:s:c")) != -1) {
		switch (opt) {
		case 'i':
			strcpy(local_ifname, optarg);
			break;
		case 's':
			add_ips(optarg);
			break;
		case 'c':
			cascade = 1;
			printf("BEWARE! cascade (-c) option can lead to routing loops!\n");
			break;
		case '1':
			one_shot = 1;
			break;
		default:
			fprintf(stderr, "Usage: %s <-i iface>  [-1] [-c] [-s X.X.X.X/Y]\n\n",
					argv[0]);
			fprintf(stderr, "  -i iface\n");
			fprintf(stderr, "      listen and send routing advertisements on interface iface\n");
			fprintf(stderr, "  -s X.X.X.X/Y\n");
			fprintf(stderr, "      advertise the subnet X.X.X.X/Y\n");
			fprintf(stderr, "  -1\n");
			fprintf(stderr, "      one-shot: send one routing advertisement and die\n");
			fprintf(stderr, "  -c\n");
			fprintf(stderr, "      cascade: re-advertise received routes.\n");
			fprintf(stderr, "      Handle with care: the -c option can lead to routing loops.\n");
			exit(1);
		}
	}

}
