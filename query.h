//ini update
#ifndef CLI2SER_H
#define CLI2SER_H

typedef struct Minor{
	char *ip;
	int port;
	char *name;
	struct Minor *next;
};

typedef struct Transaction{
	struct Minor* sender;
	struct Minor* receipient;
	double amount;
	struct Transaction* next, *prev;
};

typedef struct Ledger{
	int minor_number;
	struct Minor* minors;
	struct Transaction *head, *tail;
};

struct Minor query(int sockfd) {

	ledger = (struct Ledger*) malloc( sizeof(struct Ledger));
	char cmd[CMD_LEN] = "query";
	char msg[MSG_LEN];

	//send the "query" command to the server
	write(sockfd, cmd, CMD_LEN);

	//receiving the first reply, which is the total number
	//of active minors
	if (read( sockfd, msg, MSG_LEN) == NULL) {
		perror("Error reading minor number from server.\n");
		exit(0);
	}
	else{
		printf("Printing info about active minors\n");
		ledger->minor_number = atoi(msg);
		printf("number of minors on the network: %d\n", ledger->minor_number);
	}

	//initialize the ledger struct with reveived data
	write( sockfd, msg, MSG_LEN);
	memset(&msg, '\0', MSG_LEN);

	int i =0;
	
	char *tokens;

	struct Minor* minors = NULL;
	struct Minor* curr = minors;

	while( read( sockfd, msg, MSG_LEN) != NULL ) {
		struct Minor* minor = (struct Minor*) malloc( sizeof( struct Minor));

		printf("info of the %d's minor: %s", i, msg);
		i++;

		tokens = strtok( msg, " ");
		minor->ip = tokens;
		//printf("%s\n", minor->ip);

		tokens = strtok( NULL, " ");
		minor->port = atoi(tokens);
		//printf("%d\n", minor->port);

		tokens = strtok( NULL, " ");
		minor->name = tokens;
		//printf("%s\n", minor->name);

		minor->next = NULL;
		//add the new minor to the minors list
		if( minors == NULL ) {
			minors = minor;
		}
		else{
			curr->next = minor;
		}
		curr = minor;

		write( sockfd, msg, MSG_LEN);
		memset(&msg, '\0', MSG_LEN);
	}

	return minor;
}
#endif