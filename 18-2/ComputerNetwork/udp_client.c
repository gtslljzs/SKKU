#include "headerFiles.h"

int main(int argc, char *argv[]) {
	char *server_name = argv[1];
	int SERVER_PORT = atoi(argv[2]);

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// creates binary representation of server name and stores it as sin_addr
	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(SERVER_PORT);

	// open socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Error: socket creation failed!\n");
		return 1;
	}

	// data that will be sent to the server
	char data_to_send[256];
    memset( data_to_send, 0, sizeof(data_to_send) );
    printf( "sending: " );
    fgets( data_to_send, 255, stdin );

	// send data
	int len = sendto(sock, data_to_send, strlen(data_to_send), 0, (struct sockaddr *)&server_address, sizeof(server_address));

	// received echoed data back
	char buffer[256];
	//recvfrom(sock, buffer, len, 0, &server_address, sizeof(struct sockaddr_in) );
    recvfrom(sock, buffer, len, 0, NULL, NULL );

	buffer[len] = '\0';
	printf("recieved: %s\n", buffer);

	// close the socket
	close(sock);
	return 0;
}