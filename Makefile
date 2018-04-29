all:
	$(CC) -Wall -Werror src/server.c src/server/client.c -O2 -lpthread -o server

clean:
	$(RM) -f server

