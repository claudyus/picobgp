
#include "pbgp.h"

void let_me_spam(){
	int i;
	DEFandNULL(struct sockaddr_in, raddr);
	raddr.sin_family = PF_INET;
	raddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	raddr.sin_port = htons(9876);

	for (i=0; i < ipslen; i++) {
		
		if (sendto(sd, &(ips[i]), sizeof(bgpmsg), 0, &raddr, sizeof(struct sockaddr_in *)) == -1) {
			perror("sendto");
			exit(1);
		}
	}
}


void signal_handler (int type) {
	switch (type) {
		case SIGINT:
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

