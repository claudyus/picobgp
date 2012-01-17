
#include "pbgp.h"

void annips_add(bgpmsg info){
	time(&info.l_update);
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(++ann_len) );
	memcpy(ann_ips+ann_len, &info, sizeof(bgpmsg));
}

void annips_del(int id){
	memcpy(ann_ips+ann_len, ann_ips+id, sizeof(bgpmsg));
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(--ann_len) );
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

	ips = realloc(ips, sizeof(bgpmsg)*(++ips_len) );
	memcpy(ips+ips_len, &n_ips, sizeof(bgpmsg));
	fprintf(stderr, "add ip to ips %s\n", inet_ntoa(n_ips.addr));
}

void parse_opt(int argc, char *argv[]) {

	int opt;
	while ((opt = getopt(argc, argv, "i:s:")) != -1) {
		switch (opt) {
		case 'i':
			strcpy(local_ifname, optarg);
			break;
		case 's':
			add_ips(optarg);
			break;
		default:
			fprintf(stderr, "Usage: %s <-i iface> [-s X.X.X.X/Y]\n",
					argv[0]);
			exit(1);
		}
	}

}
