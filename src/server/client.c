#include "client.h"

void enqueue(client_t *client)
{
  if (client_count < MAX_CLIENTS) {
    clients[client_count] = client;
    client_count++;
  }
}

void dequeue(client_t *client)
{
  /* clients[client_count] = client; */
  free(client);
  client_count--;
}

int full_queue()
{
  return (client_count + 1) == MAX_CLIENTS;
}
