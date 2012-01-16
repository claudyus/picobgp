
#include "pbgp.h"

void let_me_spam(){
	int i;
	DEFandNULL(struct sockaddr_in, raddr);
	raddr.sin_family = AF_INET;
	raddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	raddr.sin_port = htons(9876);

	for (i=1; i <= ips_len; i++) {
		fprintf(stderr, "advertising %s netmask %s\n", inet_ntoa(ips[i].addr), inet_ntoa(ips[i].netmask));
		if (sendto(sd, &(ips[i]), sizeof(bgpmsg), 0, 
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
			printf("Exit due to SIGINT\n");
			exit(0);
			break;
		case SIGALRM:
			let_me_spam();
			clean_rt();		//remove old route
			alarm(3);	//and spam again...
			break;
		case SIGUSR1:
			break;
	}
}

