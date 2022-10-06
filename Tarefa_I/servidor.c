#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

#define MAX_SIZE 256
#define MAX_LINES 1000
#define MAX_CLIENTS 3 // Counting the reading thread

char text[MAX_LINES][MAX_SIZE];
pthread_t thread_id[MAX_CLIENTS];
pthread_mutex_t lock;
// strcpy(text[0], "fazendo um teste");
// strcpy(text[2], "fazendo outro teste");

struct line_addition {
  int line;
  char text[MAX_SIZE];
};

void *reading_thread(void *vargp) {
  // THREAD DE LEITURA
  int index;

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
	listen(server_sockfd, MAX_CLIENTS);
	
  while(1) {		
		printf("server waiting to read\n");
		
    client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
		
    read(client_sockfd, &index, sizeof(int));
    write(client_sockfd, text[index], MAX_SIZE);
		
    close(client_sockfd);
	}
  return NULL;
}

void *writing_thread(void *vargp) {
  // THREAD DE LEITURA
  struct line_addition la;

  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9735);
  server_len = sizeof(server_address);
  
  bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
	listen(server_sockfd, 5);
	
  while(1) {		
		printf("server waiting to write\n");
		
    client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
		
    read(client_sockfd, &la, sizeof(la));
    strcpy(text[la.line], la.text);
    int result = 0;
    pthread_mutex_lock(&lock);
    write(client_sockfd, &result, 1);
    sleep(30);
    pthread_mutex_unlock(&lock);
		
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

  pthread_create(&thread_id[0], NULL, reading_thread, NULL);
  for (size_t i = 1; i < MAX_CLIENTS; i++)
    pthread_create(&thread_id[i], NULL, writing_thread, NULL);

  for(int i = 0; i < MAX_CLIENTS; i++)
    pthread_join(thread_id[i], NULL);
  
  exit(0);
}
