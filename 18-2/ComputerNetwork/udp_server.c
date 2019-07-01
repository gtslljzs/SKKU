#include "headerFiles.h"

int main(int argc, char *argv[]) {
	// port to start the server on
	int SERVER_PORT = 1234;
    int sock;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char buffer[256];

	// socket address used for the server
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// htons: host to network short: transforms a value in host byte
	// ordering format to a short value in network byte ordering format
	server_address.sin_port = htons(SERVER_PORT);

	// htons: host to network long: same as htons but to long
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// create a UDP socket, creation returns -1 on failure
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Error: Socket creation failed!\n");
		exit(1);
	}

	// bind it to listen to the incoming connections on the created server
	// address, will return -1 on error
	if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("Error: Binding failed\n");
		return 1;
	}

	// socket address used to store client address
	int client_address_len = sizeof(struct sockaddr_in);

	// run indefinitely
	while (1) {

		// read content into buffer from an incoming client
		int len = recvfrom(sock, buffer, sizeof(buffer), 0,
                        (struct sockaddr *)&client_address, &client_address_len);

        if( len < 0 ) {
            perror( "Error: recvfrom failed!\n" );
            exit(1);
        }
		// inet_ntoa prints user friendly representation of the ip address
		
		printf("received: %s\n", buffer);

        buffer[len] = '\0';
        for( int i = 0; i < len - 1; i++ ) {
            if( buffer[i] >= 97 && buffer[i] <= 120 ) {
                buffer[i] += 2;
            } else if( buffer[i] == 121 || buffer[i] == 122 ) {
                buffer[i] -= 24;
            }
        }
		// send same content back to the client ("echo")
		sendto(sock, buffer, len, 0, (struct sockaddr *)&client_address, client_address_len);
        printf( "sended\n" );
    }

	return 0;
}