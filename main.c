
#include "pbgp.h"

int ips_len=0;
bgpmsg *ips=NULL;

int ann_len=0;
bgpmsg *ann_ips=NULL;


int main (int argc, char *argv[]) {

	parse_opt(argc, argv);

	DEFandNULL(struct sigaction, handler)
	handler.sa_handler = signal_handler;
	/*redirect Ctrl+c and signals */
	sigaction(SIGINT, &handler, 0);
	sigaction(SIGUSR1, &handler, 0);
	sigaction(SIGALRM, &handler, 0);

	/* udp socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	DEFandNULL(struct sockaddr_in, server_sock);
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(9876);
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);

	/* allow send to broadcast addr */
	int broadcast = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast,
		sizeof(int)) == -1) {
		perror("SO_BROADCAST");
		exit(1);
	}

	struct ifreq ifr;
	/*retrieve address from device */
	strncpy(ifr.ifr_name, local_ifname, IFNAMSIZ-1);
	if (ioctl(sd, SIOCGIFADDR, &ifr) == -1) {
		perror("get iface ip");
		exit(1);
	}
	loc_ip.s_addr = (*(struct sockaddr_in *)&ifr.ifr_addr).sin_addr.s_addr;
	ioctl(sd, SIOCGIFBRDADDR, &ifr);
	brd_ip.s_addr = (*(struct sockaddr_in *)&ifr.ifr_broadaddr).sin_addr.s_addr;

	/* bind on the local address of the iface required */
	if ( bind(sd, (struct sockaddr *) &server_sock, sizeof(server_sock)) < 0 ) {
		perror("bind");
		return 1;
	}

	alarm(3);	//start the spamming process

	fd_set fds;
	FD_ZERO(&fds);

	for (;;) {
		FD_SET(sd, &fds);
		/*wait for the other spammed package*/
		int r = select (sd+1, &fds, NULL, NULL, NULL);
		if (r != -1) {
			bgpmsg msg;
			struct sockaddr_in loc;
			int locl = sizeof(struct sockaddr_in);
			if ((r = recvfrom (sd, &msg, sizeof(bgpmsg), 0, (struct sockaddr *)&loc, &locl)) == -1) {
				perror("read");
				exit(1);
			}
			/* if the ip isn't the my own...*/
			if (msg.loc_addr.s_addr != loc_ip.s_addr)
				update_rt(msg, r);
		}
	}

}

