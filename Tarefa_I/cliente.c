#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define STR2(x) #x
#define STR(X) STR2(X)

void close_connection(int sockfd) {
  struct request req;
  req.type = CLS;
  write(sockfd, &req, sizeof(req));
}

void get_line(int sockfd, int index, char* text) {
  struct request req;
  req.type  = GET;
  req.index = index;
	
  write(sockfd, &req, sizeof(req));
	read(sockfd, text, MAX_SIZE);
}

int add_line(int sockfd, int index, char* text) {
  int result;

  struct request req;
  req.type  = ADD;
  req.index = index;
  strcpy(req.text, text);

  write(sockfd, &req, sizeof(req));
	read(sockfd, &result, sizeof(int));

  return result;
}

int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(9734);

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *) &address, len);
  if(result == -1) {
		perror("oops: connection error");
		exit(1);
  }

  int opcao = -1;
  while(opcao) {
    printf("[1] ler linha\n");
    printf("[2] escrever linha\n");
    printf("[0] sair\n");
    printf("\nOpcao: ");
    scanf("%d", &opcao);
    fflush(stdin);

    int index;
    char text[MAX_SIZE];
    switch(opcao) {
      case 1: printf("\n\nlinha: ");
              scanf("%d", &index);
              fflush(stdin);
              get_line(sockfd, index, text);
              printf("O texto lido eh: %s\n\n\n", text);
              break;
      
      case 2: printf("\n\nlinha: ");
              scanf("%d", &index);
              fflush(stdin);
              printf("texto: ");
              scanf(" %" STR(MAX_SIZE) "[^\n]", text);
              fflush(stdin);
              printf("\n\n");
              if(add_line(sockfd, index, text) == 1)
                printf("Erro ao inserir linha %d\n\n\n", index);
              break;
      
      case 0: close_connection(sockfd);
              break;
    }
  }

  close(sockfd);
  exit(0);
}
