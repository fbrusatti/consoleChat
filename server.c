#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main()
{
  // The sockaddr_in structure is used to store addresses for the Internet
  // protocol family 
  //   sa_family_t    sin_family (INADDR_ANY, INADDR_BROADCAST, ...)
  //   in_port_t      sin_port (An unsigned integral type of exactly 16 bits.)
  //   struct in_addr sin_addr (An unsigned integral type of exactly 32 bits.)
  //   unsigned char  sin_zero[8]
  //
  struct sockaddr_in addr;
  int fd;

  // int socket(int domain, int type, int protocol)
  // creates an unbound socket in a communications domain, and returns a file
  // descriptor that can be used in later function calls that operate on
  // sockets.
  //
  //   domain: 
  //     AF_UNIX: File system pathnames.
  //     AF_INET: Internet address.
  //
  //   type: determines the semantics of communication over the socket.
  //     SOCK_STREAM: Provides sequenced, reliable, bidirectional, 
  //                  connection-mode byte streams, and may provide a
  //                  transmission mechanism for out-of-band data.
  //     SOCK_DGRAM: Provides datagrams, which are connectionless-mode,
  //                 unreliable messages of fixed maximum length.
  //     
  //   protocol: Specifies a particular protocol to be used with the socket
  //     0 causes socket() to use an unspecified default protocol appropriate
  //     for the requested socket type
  //
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1)
  {
      printf("Error opening socket\n");
      return -1;
  }

  // htons(hostshort) (host to network short
  // returns the argument value converted from host to network byte order.
  //
  // INADDR_ANY: Local host address.
  // AF_INET: Internet address.
  //
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;
  
  // int bind(int socket, const struct sockaddr *address, socklen_t address_len)
  // assigns an address to an unnamed socket. (Sockets created with socket()
  // function are initially unnamed)
  //
  // socket: Specifies the file descriptor of the socket to be bound.
  // address: Points to a sockaddr structure containing the address to be bound
  //         to the socket. The length and format of the address depend on the
  //         address family of the socket.
  // address_len: Specifies the length of the sockaddr structure pointed to by
  // the address argument.
  //
  if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
  {
      printf("Error binding socket\n");
      return -1;
  }

  printf("Successfully bound to port %u\n", PORT);

  // int listen(int socket, int backlog);
  // marks a connection-mode socket, specified by the socket argument, as
  // accepting connections, and limits the number of outstanding connections in
  // the socket's listen queue to the value specified by the backlog argument.
  int max_waiting_clients = 3;
  if (listen(fd, max_waiting_clients) < 0)
  {
		printf("could not open socket for listening\n");
		return 1;
	}

  struct sockaddr_in client_address;
  int client_sock;
  socklen_t client_address_len = 0;
  if ((client_sock =
       accept(fd, (struct sockaddr *)&client_address, &client_address_len)) < 0)
  {
			printf("could not open a socket to accept data\n");
			return 1;
	}

  int n = 0;
	int len = 0, maxlen = 100;
	char buffer[maxlen];
	char *pbuffer = buffer;

  printf("client connected with ip address: %s\n",
		       inet_ntoa(client_address.sin_addr));

  while ((n = recv(client_sock, pbuffer, maxlen, 0)) > 0) {
		pbuffer += n;
		maxlen -= n;
		len += n;

		printf("received: '%s'\n", buffer);

		// echo received content back
		send(client_sock, buffer, len, 0);
	}

	close(client_sock);

  close(fd);

	return 0;
}

