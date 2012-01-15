
#include "pbgp.h"

int ntomask (int n) {
	return htonl(0xffffffff << (32-n));
}

void annips_add(bgpmsg info){
	time(&info.l_update);
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(++ann_len) );
	memcpy(ann_ips+ann_len, &info, sizeof(bgpmsg));
}

void annips_del(int id){
	memcpy(ann_ips+ann_len, ann_ips+id, sizeof(bgpmsg));
	ann_ips = realloc(ann_ips, sizeof(bgpmsg)*(--ann_len) );
}

void add_ips(char *cidr) {
	char *slash = index(cidr, '/');
	*slash = '\0';

	bgpmsg n_ips;
	unsigned long mask = htonl(0xffffffff << (32-atoi(slash+1)));
	memcpy (&(n_ips.netmask.s_addr), &mask , sizeof(long));
	inet_aton(cidr, &(n_ips.addr));
	n_ips.loc_addr.s_addr = loc_ip.s_addr;

	ips = realloc(ips, sizeof(bgpmsg)*(++ips_len) );
	memcpy(ips+ips_len, &n_ips, sizeof(bgpmsg));
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
