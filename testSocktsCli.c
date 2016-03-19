#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h> //DNS.
#include <sys/types.h> //tipos extendidos (adicionais). conexão telnet
#include <sys/socket.h> //funções básicas. conexão telnet
#include <netinet/in.h> // funções e tipos -
#include <arpa/inet.h> // -de dados inet/arpa.
#include <unistd.h> //funcao close
#include<time.h>

#define MINHA_PORTA 20000
#define MAXDATASIZE 10000

int main(){
  int meuSocket = socket(AF_INET,SOCK_STREAM,0);
  int numbytes, conector = 0;
  struct sockaddr_in vitima;
  char *ip = "127.0.0.1";
  char buf[MAXDATASIZE];
  char message[] = "Mensagem Recebida! \n";
  // char message[1000];

  clock_t tInicio, tFim, tDecorrido;
  if(meuSocket < 0) {
    perror("Socket");
    exit(1);
  }
  else {
    printf("Socket Criado com Sucesso! \n");
  }

  vitima.sin_family = AF_INET;
  vitima.sin_port = htons(20000);
  vitima.sin_addr.s_addr = INADDR_ANY; //inet_addr(ip);
  bzero(&(vitima.sin_zero),8);
  // memset((&endServ.sin_zero), '\0', 8);

  conector = connect(meuSocket,(struct sockaddr * )&vitima, sizeof(vitima));
	if(conector < 0){
		perror("Connect");
		exit(1);
	}

  while(1){
    // if ((numbytes = read(meuSocket,buf,strlen(MAXDATASIZE))) == -1){
    if ((numbytes =recv(meuSocket, buf, MAXDATASIZE, 0)) == -1){
      perror("recv");
      exit(1);
    }
    else {
      buf[numbytes] = '\0';
      printf("Recebido: %s \n", buf);
      if(write(meuSocket, message ,strlen(message)) < 0){
      // if( send(meuSocket , message , strlen(message) , 0) < 0){
          puts("Send failed");
          return 1;
      }
      else {
          tInicio = clock();
          // printf("Inicio: %Lf ", (long double)tInicio);
      }
      // tFim = clock();
      // printf("Fim: %Lf \n", (long double)tFim);
      // tDecorrido = (tFim - tInicio);
      // printf("Diferença: %Lf \n", (long double)tDecorrido);
    }
  }

  close(meuSocket);

  return 0;
}
