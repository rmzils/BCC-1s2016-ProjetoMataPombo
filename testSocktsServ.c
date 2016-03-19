#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define   MINHA_PORTA   20000
#define   BACKLOG       5
#define   MAXDATASIZE   10000

int main(){
  int meuSocket, novoSocket, numbytes;
  struct sockaddr_in local, remote;
	unsigned int tamanho = sizeof(struct sockaddr_in);
  char buf[MAXDATASIZE];

  meuSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(meuSocket < 0){
		perror("\nErro ao criar socket.\n");
		exit(1);
	}
	else{
		printf("\nSocket criado com sucesso.\n");
	}

  local.sin_family = AF_INET;
	local.sin_port = htons(MINHA_PORTA);
	//endServ.sin_addr.s_addr = inet_addr(ip);
	local.sin_addr.s_addr = INADDR_ANY;
	memset((&local.sin_zero), '\0', 8);

	bind(meuSocket, (struct sockaddr *)&local, sizeof(local));
	listen(meuSocket, BACKLOG);
	printf("\nSocket aguardando conexoes. Seguem os dados:\n");
	printf("IP: %s\n", inet_ntoa(local.sin_addr));
	printf("Porta: %d\n", ntohs(local.sin_port));

  char message[] = "Obrigado!";

  novoSocket = accept(meuSocket, (struct sockaddr *)&remote, &tamanho);
  if(novoSocket == 1){
    perror("\nErro ao aceitar conexao do usuario.\n");
    exit(1);
  }
  else {
    printf("Conectado pelo cliente: \nIP: %s", inet_ntoa(remote.sin_addr));
    printf("\nPorta: %d\n", ntohs(remote.sin_port));
  }

	while(1){

    if ((numbytes=recv(novoSocket, buf, MAXDATASIZE, 0)) == -1){
      perror("recv");
      exit(1);
    }
    else {
      buf[numbytes] = '\0';
      printf("Recebido: %s \n", buf);

      if(write(novoSocket,message,strlen(message)) < 0)
    	{
    		printf("Erro em write()\n");
    		close(novoSocket);
    		exit(-1);
    	}

      // if (send(novoSocket, message, 16, 0) == -1)
      // {
      //   perror("send");
      //   close(novoSocket);
      //   exit(0);
      // }
    }
	}
  close(novoSocket);
	close(meuSocket);

	return 0;
}
