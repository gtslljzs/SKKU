// TCP echo client program
#include "headerFiles.h"

int main( int argc, char *argv[] ) {
    char *server_name;           // server name
    int SERVER_PORT;             // server port number
    char *string;                // string to be echoed
    struct sockaddr_in server_address; // server socket address
    int sock;                          // socket descriptor
    char buffer[256 + 1];              // Buffer
    char *pbuffer = buffer;            // pointer to momve along the buffer
    int n = 0;                         // number of bytes in each received call
    int len = 0;                       // length of string to be echoed
    int max_len = 256;

    // Check and set arguments
    if( argc != 3 ) {
        printf( "Error: At least three arguments are needed!\n" );
        exit(1);
    }
    server_name = argv[1];
    SERVER_PORT = atoi(argv[2]);

    // Create remote (server) socket address
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;

    // Create binary representation of server name and stores it as sin_addr
    inet_pton( AF_INET, server_name, &server_address.sin_addr );

    // htons: port in network order format
    server_address.sin_port = htons(SERVER_PORT);

    // Create socket
    if( (sock = socket(PF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror( "Error: Socket creation failed!\n" );
        exit(1);
    }

    // Connect to the server
    if( connect( sock, (struct sockaddr *)&server_address, sizeof(server_address) ) < 0 ) {
        perror( "Error: connection failed!\n" );
        exit(1);
    }

    // Data transfer section
    printf( "sending: " );
    fgets( string, max_len, stdin );
    send( sock, string, strlen(string), 0 );

    while( (n = recv(sock, pbuffer, max_len, 0) ) > 0 ) {
        printf( "\nreceiving...\n" );
        max_len -= n;
        len += n;
        buffer[len] = '\0';
        
        printf( "Echo string received: %s\n", pbuffer );
        pbuffer += n;

        printf( "sending: " );
        fgets( string, max_len, stdin );
        send( sock, string, strlen(string), 0 );
    }

    // Close the socket
    close(sock);

    return 0;
}