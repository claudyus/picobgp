/* picobgp - tiny routing advertising daemon
 *
 * Copyright 2012 - Claudio Mignanti <c.mignanti@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License. */

#include "pbgp.h"

int ips_len=0;
bgpmsg *ips=NULL;

int ann_len=0;
bgpmsg *ann_ips=NULL;

int cascade = 0;
int one_shot = 0;	/*one-shot and die mode */

int main (int argc, char *argv[]) {

	/* udp socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1) {
		perror("socket(SOCK_DGRAM)");
		exit(1);
	}

	parse_opt(argc, argv);

	DEFandNULL(struct sigaction, handler)
	handler.sa_handler = signal_handler;
	/*redirect Ctrl+c and signals */
	sigaction(SIGINT, &handler, 0);
	sigaction(SIGUSR1, &handler, 0);
	sigaction(SIGALRM, &handler, 0);

	DEFandNULL(struct sockaddr_in, server_sock);
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(9876);
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);

	/* allow send to broadcast addr */
	int broadcast = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast,
		sizeof(int)) == -1) {
		perror("setsockopt(SO_BROADCAST)");
		exit(1);
	}

	/* bind on the local address of the iface required */
	server_sock.sin_addr.s_addr = loc_ip.s_addr;
	if ( bind(sd, (struct sockaddr *) &server_sock, sizeof(server_sock)) < 0 ) {
		perror("bind(&server_sock)");
		return 1;
	}

	alarm(3);	//start the spamming process

	fd_set fds;
	FD_ZERO(&fds);

	for (;!one_shot;) {
		FD_SET(sd, &fds);
		/*wait for the other spammed package*/
		int r = select (sd+1, &fds, NULL, NULL, NULL);
		if (r != -1) {
			bgpmsg msg;
			struct sockaddr_in loc;
			int locl = sizeof(struct sockaddr_in);
			if ((r = recvfrom (sd, &msg, sizeof(bgpmsg), 0, (struct sockaddr *)&loc, &locl)) == -1) {
				perror("read(&msg)");
				exit(1);
			}
			/* if the ip isn't the my own...*/
			if (msg.loc_addr.s_addr != loc_ip.s_addr)
				update_rt(msg, r);
		}
	}

	/* implement the one-shot and die mode */
	let_me_spam();
	return 0;

}

