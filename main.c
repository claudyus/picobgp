
#include "pbgp.h"

int main (int argc, char *argv[]) {

	DEFandNULL(struct sigaction, handler);
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

	/*retrieve address from device */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);	//FIXME getopt da ste parti
	ioctl(sd, SIOCGIFADDR, &server_sock);

	if ( bind(sd, (struct sockaddr*) &server_sock, sizeof(server_sock)) < 0 ) {
		perror("bind");
		return 1;
	}

	/* bind the route socket */
	rd = socket(AF_INET, SOCK_RAW, NETLINK_ROUTE);
	DEFandNULL(struct sockaddr_nl, la);
	la.nl_family = AF_NETLINK;
	la.pid = getpid();
	if (bind(rd, &la, sizeof(la)) == -1) {
		perror ("route bind");
		return 1;
	}

	alarm(3);	//start the spamming process


	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sd, &fds);

	for (;;) {
		/*wait for the other spammed package*/
		int r = select (sd+1, &fds, NULL, NULL, NULL);
		if (r == -1) {
			perror ("select");
			exit(1);
		} else {
			
		}
	}

}

