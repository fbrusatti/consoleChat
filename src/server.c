/*
 *
 * Use mode: server [PORT] [MAX_CLIENTS]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "server/client.h"

#define DEFAULT_PORT 4848
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

static int uid = 10;


/* Print ip address */
void print_client_addr(struct sockaddr_in addr)
{
  printf("%d.%d.%d.%d",
    addr.sin_addr.s_addr & 0xFF,
    (addr.sin_addr.s_addr & 0xFF00)>>8,
    (addr.sin_addr.s_addr & 0xFF0000)>>16,
    (addr.sin_addr.s_addr & 0xFF000000)>>24);
}

// MANAGE MESSAGES
/* Send message to all clients */
void broadcast(char *message)
{
  int i;
  for(i = 0; i < MAX_CLIENTS; i++) {
    if(clients[i]) {
      write(clients[i]->conn_fd, message, strlen(message));
    }
  }
}

/* Send message to all clients but the sender */
void send_message(char *msge, int uid)
{
  char *message = (char *) malloc(strlen(message) + 6 * sizeof(char));
  sprintf(message, "[%d] %s", uid, msge);
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if(clients[i]) {
      if(clients[i]->uid != uid) {
        write(clients[i]->conn_fd, message, strlen(message));
      }
    }
  }
}

void send_message_to_client(const char *s, int conn_fd){
  write(conn_fd, s, strlen(s));
}

/* Strip CRLF */
void strip_newline(char *s)
{
  while(*s != '\0') {
    if(*s == '\r' || *s == '\n') {
      *s = '\0';
    }
    s++;
  }
}

/* Threaded function */
/* Handle the communication with the client */
void *connection_handler(void *arg)
{
  char buff_out[BUFFER_SIZE];
  char buff_in[BUFFER_SIZE];
  int rlen;

  client_t *client = (client_t *) arg;

  printf("[CONNECTION ACCEPTED] Client uid %d from ", client->uid);
  print_client_addr(client->addr);
  printf("\n");

  sprintf(buff_out, "You've been joinned, HELLO %s\r\n", client->name);
  send_message_to_client(buff_out, client->conn_fd);

  sprintf(buff_out, "Please all, say hi to %s \r \n", client->name);
  send_message(buff_out, client->uid);

  while(1) {
    rlen = recvfrom(client->conn_fd, buff_in, BUFFER_SIZE, 0, NULL, NULL);
    buff_in[rlen] = '\0';
    buff_out[0] = '\0';
    strip_newline(buff_in);

    printf("LOG [RECEIVED MESSAGE] %s \n", buff_in);

    /* Ignore empty buffer */
    if(!strlen(buff_in)) {
      continue;
    }

    /* Send message */
    sprintf(buff_out, "%s \r\n", buff_in);
    send_message(buff_out, client->uid);
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  /* The sockaddr_in structure is used to store addresses for the Internet
   * protocol family
   *   sa_family_t    sin_family (INADDR_ANY, INADDR_BROADCAST, ...)
   *   in_port_t      sin_port (An unsigned integral type of exactly 16 bits.)
   *   struct in_addr sin_addr (An unsigned integral type of exactly 32 bits.)
   *   unsigned char  sin_zero[8] */
  struct sockaddr_in addr;
  struct sockaddr_in client_addr;
  int sock_fd, client_sock_fd = 0;
  int portno = (argc < 2 ? DEFAULT_PORT : atoi(argv[1]));
  pthread_t tid;

  /* Socket Settings */
  /* int socket(int domain, int type, int protocol)
   * creates an unbound socket in a communications domain, and returns a file
   * descriptor that can be used in later function calls that operate on
   * sockets.
   *
   *   domain:
   *     AF_UNIX: File system pathnames.
   *     AF_INET: Internet address.
   *
   *   type: determines the semantics of communication over the socket.
   *     SOCK_STREAM: Provides sequenced, reliable, bidirectional,
   *                  connection-mode byte streams, and may provide a
   *                  transmission mechanism for out-of-band data.
   *     SOCK_DGRAM: Provides datagrams, which are connectionless-mode,
   *                 unreliable messages of fixed maximum length.
   *
   *   protocol: Specifies a particular protocol to be used with the socket
   *     0 causes socket() to use an unspecified default protocol appropriate
   *     for the requested socket type */
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_fd == -1) {
    printf("Error opening socket\n");
    return -1;
  }
  /* htons(hostshort) (host to network short
   * returns the argument value converted from host to network byte order.
   *
   * INADDR_ANY: Local host address.
   * AF_INET: Internet address. */
  addr.sin_port = htons(portno);
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;

  /* Bind */
  /* int bind(int socket, const struct sockaddr *address, socklen_t address_len)
   * assigns an address to an unnamed socket. (Sockets created with socket()
   * function are initially unnamed)
   *
   * socket: Specifies the file descriptor of the socket to be bound.
   * address: Points to a sockaddr structure containing the address to be bound
   *         to the socket. The length and format of the address depend on the
   *         address family of the socket.
   * address_len: Specifies the length of the sockaddr structure pointed to by
   * the address argument. */
  if(bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    printf("Error binding socket\n");
    return -1;
  }
  printf("Successfully bound to port %u\n", portno);

  /* Listen */
  /* int listen(int socket, int backlog);
   * marks a connection-mode socket, specified by the socket argument, as
   * accepting connections, and limits the number of outstanding connections in
   * the socket's listen queue to the value specified by the backlog argument. */
  int max_waiting_clients = 5;
  if (listen(sock_fd, max_waiting_clients) < 0) {
		printf("could not open socket for listening\n");
		return 1;
	}
  printf("Server started...\n");

  /* Accept Clients */
  socklen_t client_addr_len = sizeof(struct sockaddr_in);;
  while(1) {

    /* extracts the first connection on the queue of pending connections, creates a new socket with the same socket
     * type protocol and address family as the specified socket, and allocates a new file descriptor for that socket.
     *
     * socket: Specifies a socket that was created with socket(), has been bound to an address with bind(), and has
     *         issued a successful call to listen().
     * address: Either a null pointer, or a pointer to a sockaddr structure where the address of the connecting socket
     *          will be returned.
     * address_len: Points to a socklen_t which on input specifies the length of the supplied sockaddr structure, and
     *              on output specifies the length of the stored address. */
    if ((client_sock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
      printf("could not open a socket to accept data\n");
      return 1;
    }

    /* Check if client queue is full */
    if(full_queue()) {
      printf("[CONNECTION REJECT] Max client reached \n\n");
      print_client_addr(client_addr);
      printf("\n");
      close(client_sock_fd);
      continue;
    }

    /* Client settings */
    client_t *client = (client_t *) malloc(sizeof(client_t));
    client->addr = client_addr;
    client->conn_fd = client_sock_fd;
    client->uid = uid++;
    sprintf(client->name, "%d", client->uid);

    /* Add client to the queue and fork thread */
    enqueue(client);
    pthread_create(&tid, NULL, &connection_handler, (void*)client);

    /* Calm down the CPU */
    sleep(1);
  }
}
