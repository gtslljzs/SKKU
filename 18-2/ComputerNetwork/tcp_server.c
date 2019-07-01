// TCP echo server program
#include "headerFiles.h"

int main() {
    int SERVER_PORT = 1234;            // port to start the server on
    struct sockaddr_in server_address; // server address
    struct sockaddr_in client_address; // client address
    int client_address_len = 0;        // length of client address
    int ls;                            // listen socket
    int wait_size = 16;                // the number of waiting clients
    int sock;                          // socket decriptor (reference)

    // socket address used for the server
    memset( &server_address, 0, sizeof(server_address) );
    server_address.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons( SERVER_PORT );

    // htonl: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl( INADDR_ANY ); // default IP address

    // Create a TCP listen socket
    if( ( ls = socket(PF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror( "Error: Listen socket creation failed!\n" );
        exit(1);
    }

    // Bind listen socket to the local socket address
    if( bind( ls, (struct sockaddr *)&server_address, sizeof(server_address) ) < 0 ) {
        perror( "Error: Binding failed!\n" );
        exit(1);
    }

    // Listen to connection requests
    if( listen(ls, wait_size) < 0 ) {
        perror( "Error: Listening failed!\n" );
        exit(1);
    }

    fputs( "waiting client...\n", stdout );

    // Run forever and Handle the connection
    while( 1 ) {
        if( (sock = accept(ls, (struct sockaddr *)&client_address, &client_address_len) ) < 0 ) {
            perror( "Error: Accepting failed!\n" );
            exit(1);
        }

        printf( "client connected with ip address: %s\n", inet_ntoa(client_address.sin_addr) );

        // Data transfer section
        char buffer[256];                  // data buffer
        memset( buffer, 0, sizeof(buffer) );
        char *pbuffer = buffer;            // data buffer
        int n = 0;                         // number of bytes for each received call
        int len = 0;                       // number of bytes to send or receive
        int max_len = sizeof(buffer);      // maximum number of bytes
        
        // Keep running as long as the client keeps the connection open
        while( (n = recv(sock, pbuffer, max_len, 0) ) > 0 ) {
            printf( "received: %s\n", pbuffer );
            max_len -= n;
            len = n;
            
            // Send back (echo) all bytes received
            int i;
            for( int i = 0; i < len - 1; i++ ) {
                if( *(pbuffer + i) >= 97 && *(pbuffer + i) <= 120 ) {
                    *(pbuffer + i) += 2;
                } else if( *(pbuffer + i) == 121 || *(pbuffer + i) == 122 ) {
                    *(pbuffer +i) -= 24;
                }
            }
            send( sock, pbuffer, len, 0 );
            pbuffer += n;

            printf( "message sended\n\n" );
        }
        printf( "all received\n\n" );
        fputs( "waiting client...\n", stdout );


        // Close the socket
        close(sock);
    }

    // Close the listen socket
    close(ls);

    return 0;
}