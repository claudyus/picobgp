
#include "pbgp.h"

int ipslen=0;
bgpmsg *ips=NULL;

int ann_len=0;
bgpmsg *ann_ips=NULL;


int main (int argc, char *argv[]) {

	parse_opt(argc, argv);

	DEFandNULL(struct sigaction, handler)
	handler.sa_handler = signal_handler;
	//redireziono Ctrl+c
	sigaction(SIGINT, &handler, 0);
	sigaction(SIGUSR1, &handler, 0);
	sigaction(SIGALRM, &handler, 0);

	/* udp socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	DEFandNULL(struct sockaddr_in, server_sock);
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(9876);

	struct ifreq ifr;
	/*retrieve address from device */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(sd, SIOCGIFADDR, &ifr);
	//server_sock.sin_addr= (struct sockaddr_in *)ifr.ifr_addr.sin_addr;

	if ( bind(sd, (struct sockaddr*) &server_sock, sizeof(server_sock)) < 0 ) {
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
		if (r == -1) {
			perror ("select");
			exit(1);
		} else {
			bgpmsg msg;
			if (read (sd, &msg, sizeof(bgpmsg)) == -1) {
				perror("read");
				exit(1);
			}
			update_rt(&msg);
		}
	}

}

