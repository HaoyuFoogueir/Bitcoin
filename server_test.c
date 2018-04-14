#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define MSG_LEN 256
#define CMD_LEN 20
#define SERVER_PORT 2000

void ex_file(int conn_sock);
int main(int argc, char const *argv[])
{
	printf("server running\n");

	//create a socket
	int in_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (in_sock < 0){
		perror("Error creating socket");
		exit(0);
	}
	
	//set up the server addrress
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '\0', sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);


	if ( bind(in_sock, (struct sockaddr *) &serv_addr, sizeof serv_addr) < 0) {
		perror("Error binding");
		exit(0);		
	}

	while(1) {

		if( listen(in_sock, 10) < 0) {
			perror("error listening\n");
			exit(0);
		}

		//accept incoming connection
		struct sockaddr_in cli_addr;
		int cli_len = sizeof(cli_addr);
		int conn_sock = accept( in_sock, (struct sockaddr *) &cli_addr, &cli_len);

		//testing to see if connection is successful
		char test_msg[30];
		read(  conn_sock, test_msg, 30);
		printf("%s\n", test_msg);
		strcpy( test_msg, "connected to server.");
		write( conn_sock, test_msg, 30);

		//read the command from client
		char cmd[CMD_LEN];
		read( conn_sock, cmd, CMD_LEN);

		switch( tolower(cmd[0]) ) {

			case 'q':
				ex_file(conn_sock);
				break;

			default:
				printf("Wrong Command\n");
		}
	
		close( conn_sock);
	}

	putchar('\n');
	return 0;
}

void ex_file(int conn_sock) {

	char msg[MSG_LEN];

	//open database for reading
	FILE *data_file = fopen("data.txt", "r");
	if( data_file == NULL ) {
		perror("\nFail to open database.\n");
		exit(0);
	}
	else{
		printf("File opened.\n");
	}

	//return the first line of the file
	while( fgets( msg, MSG_LEN, data_file) != NULL) {
		printf("%s", msg);
		write( conn_sock, msg, MSG_LEN);
		read( conn_sock, msg, MSG_LEN);
	}
	fclose(data_file);
}