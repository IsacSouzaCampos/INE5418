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
int count_clients;
pthread_mutex_t write_lock, count_lock;


void* keep_connection(void* p_client_sockfd) {
  int client_sockfd = *((int*) p_client_sockfd);
  free(p_client_sockfd);
  struct request req;
  
  while(1) {
    printf("Socket esperando\n");
    read(client_sockfd, &req, sizeof(req));
    
    if(req.type == GET) {
      printf("Vai ler\n");
      write(client_sockfd, text[req.index], MAX_SIZE);
      printf("Leu \"%s\" da linha %d\n", text[req.index], req.index);
    }
    else if(req.type == ADD) {
      printf("Vai escrever\n");
      int result = 0;
      pthread_mutex_lock(&write_lock);
      strcpy(text[req.index], req.text);
      pthread_mutex_unlock(&write_lock);
      
      write(client_sockfd, &result, sizeof(int));
      printf("Escreveu \"%s\" na linha %d\n", text[req.index], req.index);
    }
    else if(req.type == CLS)
      break;
  }

  close(client_sockfd);

  pthread_mutex_lock(&count_lock);
  count_clients--;
  printf("Clients logados: %d\n", count_clients);
  pthread_mutex_unlock(&count_lock);

  return NULL;
}

int main()
{
  count_clients = 0;
  
  if (pthread_mutex_init(&write_lock, NULL) || pthread_mutex_init(&count_lock, NULL)) {
    perror("error: could not create mutex");
    exit(1);
  }

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

    if(count_clients >= MAX_CLIENTS) {
      printf("Numero de clients excedido\n");
      int result = -1;
      write(client_sockfd, &result, sizeof(int));
      close(client_sockfd);
      continue;
    } else {
      printf("Conexao ok\n");
      int result = 0;
      write(client_sockfd, &result, sizeof(int));
    }

    pthread_t t;
    int *p_client_sockfd = malloc(sizeof(int));
    *p_client_sockfd = client_sockfd;
    pthread_create(&t, NULL, keep_connection, p_client_sockfd);

    pthread_mutex_lock(&count_lock);
    count_clients++;
    printf("Clients logados: %d\n", count_clients);
    pthread_mutex_unlock(&count_lock);
  }
  
  exit(0);
}
