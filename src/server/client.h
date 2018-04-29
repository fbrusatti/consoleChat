#ifndef _server_client_h
#define _server_client_h

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 100
#endif

#include <stdlib.h>
#include <arpa/inet.h>

static unsigned int client_count = 0;

/* Client structure */
typedef struct {
  struct sockaddr_in addr;  /* Client remote address */
  int conn_fd;              /* Connection file descriptor */
  int uid;                  /* Client unique identifier */
  char name[32];            /* Client name */
} client_t;
client_t *clients[MAX_CLIENTS];

void enqueue(client_t *client);

void dequeue(client_t *client);

int full_queue();

#endif
