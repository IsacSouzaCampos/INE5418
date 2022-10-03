#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define STR2(x) #x
#define STR(X) STR2(X)

#define MAX_SIZE 256

char* get_line(int index) {
  printf("\n\nA linha a ser lida eh: %d\n", index);
  return "Texto de teste\n\n\n";
}

void add_line(int index, char text[MAX_SIZE]) {
  printf("\n\nA linha a ser alterada eh: %d\n", index);
  printf("O texto a ser adicionado eh: %s\n\n\n", text);
}

int main()
{
  int opcao = -1;
  while(opcao) {
    printf("[1] ler linha\n");
    printf("[2] escrever linha\n");
    printf("[0] sair\n");
    printf("\nOpcao: ");
    scanf("%d", &opcao);
    fflush(stdin);

    int index;
    switch(opcao) {
      case 1: printf("\n\nlinha: ");
              scanf("%d", &index);
              fflush(stdin);
              printf("O texto lido eh: %s\n", get_line(index));
              break;
      
      case 2: char text[MAX_SIZE];
              printf("\n\nlinha: ");
              scanf("%d", &index);
              fflush(stdin);
              printf("texto: ");
              scanf(" %" STR(MAX_SIZE) "[^\n]", text);
              fflush(stdin);
              add_line(index, text);
              break;
    }
  }

  return 0;

  // int sockfd;
  // int len;
  // struct sockaddr_in address;
  // int result;
  // char ch = 'A';

  // sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // address.sin_family = AF_INET;
  // address.sin_addr.s_addr = inet_addr("127.0.0.1");
  // // address.sin_addr.s_addr = inet_addr("192.168.0.15");
  // //address.sin_addr.s_addr = htonl(INADDR_ANY);
  // address.sin_port = htons(9734);

  // len = sizeof(address);
  // result = connect(sockfd, (struct sockaddr *)&address, len);
  // if(result == -1) {
	// 	perror("oops: client1");
	// 	exit(1);
  // }
	// write(sockfd, &ch, 1);
	// // sleep(5);
	// read(sockfd, &ch, 1);
	// printf("char from server = %c\n", ch);
	// close(sockfd);
	// exit(0);
}
