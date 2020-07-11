#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define STDIN 0
#define PORT 4848
#define SIZE 1024
#define SERVER_ADDR "127.0.0.1"

char buffer[SIZE];

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

        /* create endpoint */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                perror("socket creation failed");
                exit(1);
        }

        /* connect to server */
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        serv_addr.sin_port = htons(PORT);

        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                perror("connect to server failed");
                exit(3);
        }
        printf("Connected\n");

        while (1) {
                /*
                 * we need to select between messages FROM the user on the
                 * console and messages TO the user from the socket
                */
                FD_CLR(sockfd, &readfds);
                FD_SET(sockfd, &readfds);
                FD_SET(STDIN, &readfds);
                printf("Selecting\n");
                select(sockfd+1, &readfds, NULL, NULL, NULL);

                if (FD_ISSET(STDIN, &readfds)) {
                        printf("reading from stdin\n");
                        nread = read(0, buffer, SIZE);
                        if (nread <= 0 )
                                break;
                        sendto(sockfd, buffer, nread, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

                } else if (FD_ISSET(sockfd, &readfds)) {
                        printf("Reading from socket\n");
                        len = sizeof(serv_addr);
                        /*
                         * int recvmsg(int socket, struct msghdr *msg, int flags)
                         */
                        nread = recvmsg(sockfd, &msg, 0);
                        write(1, buffer, nread);
                }
        }
        close(sockfd);
        exit(0);
}

