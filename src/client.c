/*
 * Chat client - connects to a chat server and allows bidirectional messaging
 *
 * Usage: ./client [SERVER_ADDR] [PORT]
 *
 * The client uses select() to multiplex between:
 *   - User input from stdin (console)
 *   - Incoming messages from the server (socket)
 *
 * Type messages to send to the server, receive broadcast messages from other users.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define STDIN 0         /* File descriptor for standard input */
#define PORT 4848       /* Default server port */
#define SIZE 1024      /* Buffer size for messages */
#define SERVER_ADDR "127.0.0.1"  /* Default server address */

char buffer[SIZE];

/*
 * check_exit - checks if message contains the exit character '#'
 *
 * Returns: 0 if exit char found (should disconnect), 1 otherwise
 */
int check_exit(const char *msg) {
	for (int i = 0; i < strlen(msg); ++i) {
		if (msg[i] == '#')
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
        int sockfd;
        int nread;
        int len;
        struct sockaddr_in serv_addr;
        fd_set readfds;
        struct msghdr msg;

        /* Create TCP socket endpoint for communication */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                perror("socket creation failed");
                exit(1);
        }

        /* Configure server address structure */
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        serv_addr.sin_port = htons(PORT);

        /* Establish connection to chat server */
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                perror("connect to server failed");
                exit(3);
        }
        printf("Connected\n");

        /* Main event loop - multiplex between stdin and socket using select() */
        while (1) {
                /* Reset and set file descriptors for select()
                 * select() blocks until input is available on either stdin or socket
                 * This allows concurrent reading from console and receiving from server */
                FD_CLR(sockfd, &readfds);
                FD_SET(sockfd, &readfds);
                FD_SET(STDIN, &readfds);
                printf("Selecting\n");
                select(sockfd+1, &readfds, NULL, NULL, NULL);

                /* Handle user input from console - send to server */
                if (FD_ISSET(STDIN, &readfds)) {
                        printf("reading from stdin\n");
                        nread = read(0, buffer, SIZE);
                        if (nread <= 0 )
                                break;
                        sendto(sockfd, buffer, nread, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

                /* Handle incoming message from server - display to console */
                } else if (FD_ISSET(sockfd, &readfds)) {
                        printf("Reading from socket\n");
                        len = sizeof(serv_addr);
                        /*
                         * recvmsg() receives a message from the socket
                         * struct msghdr contains message metadata (addresses, control data, etc.)
                         */
                        nread = recvmsg(sockfd, &msg, 0);
                        write(1, buffer, nread);
                }
        }
        close(sockfd);
        exit(0);
}

