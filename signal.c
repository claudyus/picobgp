
#include "pbgp.h"

void let_me_spam(){
	if (sendto(sd, buffer, , 0, &, )==-1)
		perror("sendto");
		exit(1);
	}

}


void signal_handler (int type) {
	switch (type) {
		case SIGINT:
			break;
		case SIGALRM:
			let_me_spam();
			alarm(3);	//and spam again...
			break;
		case SIGUSR1:
			break;
	}
}

