/*
 * Client management header for chat server
 *
 * Defines the client_t structure and client management functions
 * for tracking connected chat participants.
 */

#ifndef _server_client_h
#define _server_client_h

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 100   /* Maximum number of concurrent clients */
#endif

#include <stdlib.h>
#include <arpa/inet.h>


/*
 * client_t - represents a connected chat client
 *
 * Members:
 *   addr:     Remote socket address of the client
 *   conn_fd:  File descriptor for the client connection
 *   uid:      Unique identifier assigned by the server
 *   name:     Client's display name (max 31 chars + null terminator)
 */
typedef struct {
  struct sockaddr_in addr;  /* Client remote address */
  int conn_fd;              /* Connection file descriptor */
  int uid;                  /* Client unique identifier */
  char name[32];            /* Client name */
} client_t;

/* External declaration of the clients array (defined in client.c) */
extern client_t *clients[MAX_CLIENTS];

/* Client management function declarations */
void enqueue(client_t *client);

void dequeue(client_t *client);

int full_queue();

#endif
