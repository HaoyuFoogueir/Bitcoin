#include	<sys/types.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<unistd.h>

#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>

#include "cli2ser.h"

#define CMD_LEN 20
#define MSG_LEN 256
#define DATA_FILE "client_data.txt"

struct Ledger *ledger;

void query(int);
int main(int argc, char* argv[]) {

	if( argc != 2) {
		perror("wrong format\n");
		exit(0);
	}

	//ledger = (struct Ledger *) malloc( sizeof ( struct Ledger));


	int port_number = atoi(argv[1]);
	printf("%d\n", port_number);

	int sockfd = socket( AF_INET, SOCK_STREAM, 0);

	//initialize client addr/port

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(2000);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);

	connect( sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr));

	//testing for connection
	char test_msg[30] = "connected to client.\n";
	write( sockfd, test_msg, 30);
	read( sockfd, test_msg, 30);
	printf("%s\n", test_msg);

	//query for initial minor information, and store it in a local data file
	query(sockfd);

	close(sockfd);
	putchar('\n');
	return 0;
}