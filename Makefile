all:
	$(CC) -Wall -Werror src/server.c -O2 -lpthread -o server

clean:
	$(RM) -f server

