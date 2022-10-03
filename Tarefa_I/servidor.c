#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#define MAX_SIZE 256
#define MAX_LINES 1000

int main()
{
  char text[MAX_LINES][MAX_SIZE];
  int index;
  // strcpy(text[0], "fazendo um teste");
  // strcpy(text[2], "fazendo outro teste");
  
  // THREAD DE LEITURA
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  
  bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
	listen(server_sockfd, 5);
	
  while(1) {		
		printf("server waiting\n");
		
    client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
		
    read(client_sockfd, &index, sizeof(int));
    write(client_sockfd, text[index], MAX_SIZE);
		
    close(client_sockfd);
	}
}
