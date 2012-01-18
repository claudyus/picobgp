/* picobgp - tiny routing advertising daemon
 *
 * Copyright 2012 - Claudio Mignanti <c.mignanti@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License. */

#include "pbgp.h"

void let_me_spam(){
	int i;
	DEFandNULL(struct sockaddr_in, raddr);
	raddr.sin_family = AF_INET;
	raddr.sin_addr.s_addr = brd_ip.s_addr;
	raddr.sin_port = htons(9876);

	for (i=1; i <= ips_len; i++) {
		fprintf(stderr, "advertising %s netmask %s\n", inet_ntoa(ips[i].addr), inet_ntoa(ips[i].netmask));
		if (sendto(sd, &(ips[i]), sizeof(bgpmsg), 0, 
				(struct sockaddr *) &raddr, sizeof(struct sockaddr_in)) == -1) {
			perror("sendto(&(ips[i]))");
			exit(1);
		}
	}
	for (i=1; i <= ann_len && cascade; i++) {
		fprintf(stderr, "advertising cascate subnet %s\n", inet_ntoa(ann_ips[i].addr));
		if (sendto(sd, &(ann_ips[i]), sizeof(bgpmsg), 0,
				(struct sockaddr *) &raddr, sizeof(struct sockaddr_in)) == -1) {
			perror("sendto(&(ips[i]))");
			exit(1);
		}
	}
}


void signal_handler (int type) {
	switch (type) {
		case SIGINT:
			close(sd);
			clean_rt(1);	//remove all route
			printf("Exit due to SIGINT\n");
			exit(0);
			break;
		case SIGALRM:
			let_me_spam();
			clean_rt(0);		//remove old route
			alarm(3);	//and spam again...
			break;
		case SIGUSR1:
			break;
	}
}

