/*
 * Client management functions for the chat server
 *
 * Manages a queue/list of connected clients and provides operations
 * to add, remove, and query the client collection.
 */

#include "client.h"

unsigned int client_count = 0;

/* Array storing pointers to all connected clients */
client_t *clients[MAX_CLIENTS];

/*
 * enqueue - adds a new client to the active clients list
 *
 * client: pointer to the client_t structure to add
 */
void enqueue(client_t *client)
{
  if (client_count < MAX_CLIENTS) {
    clients[client_count] = client;
    client_count++;
  }
}

/*
 * dequeue - removes a client from the active clients list and frees memory
 *
 * client: pointer to the client_t structure to remove
 */
void dequeue(client_t *client)
{
  for (int i = 0; i < client_count; i++) {
    if (clients[i] == client) {
      free(client);
      for (int j = i; j < client_count - 1; j++) {
        clients[j] = clients[j + 1];
      }
      clients[client_count - 1] = NULL;
      client_count--;
      return;
    }
  }
}

/*
 * full_queue - checks if the client queue has reached capacity
 *
 * Returns: 1 if queue is full (at MAX_CLIENTS - 1, leaving one slot),
 *          0 otherwise
 */
int full_queue()
{
  return (client_count + 1) == MAX_CLIENTS;
}
