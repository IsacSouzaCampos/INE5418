#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

#include "utils.h"

char text[MAX_LINES][MAX_SIZE];
pthread_t thread_id[MAX_CLIENTS];
pthread_mutex_t lock;

void *access_text(void *vargp) {
  struct request req;

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
    printf("Servidor esperando\n");
    client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
		
    read(client_sockfd, &req, sizeof(req));
    if(req.type == GET) {
      printf("Vai ler\n");
      write(client_sockfd, text[req.index], MAX_SIZE);
      printf("Leu \"%s\" da linha %d\n", text[req.index], req.index);
    }
    else if(req.type == ADD) {
      printf("Vai escrever\n");
      int result = 0;
      pthread_mutex_lock(&lock);
      strcpy(text[req.index], req.text);
      pthread_mutex_unlock(&lock);
      
      write(client_sockfd, &result, 1);
      printf("Escreveu \"%s\" na linha %d\n", text[req.index], req.index);
    }		
    close(client_sockfd);
	}
  return NULL;
}

int main()
{
  if (pthread_mutex_init(&lock, NULL)) {
    perror("error: could not create mutex");
    exit(1);
  }

  for (size_t i = 0; i < MAX_CLIENTS; i++)
    pthread_create(&thread_id[i], NULL, access_text, NULL);

  for(size_t i = 0; i < MAX_CLIENTS; i++)
    pthread_join(thread_id[i], NULL);
  
  exit(0);
}
