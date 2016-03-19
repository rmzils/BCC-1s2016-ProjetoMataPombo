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

// int main(){

int main(int argc, char *argv[]) {

  int numeroSend;
  if(argc == 2){
    numeroSend = atoi(argv[1]);
    printf("Quantidade de Mensagens: %d\n", numeroSend );
  }
  else {
    printf("Erro! \nUso: Digite número de mensagens que e servidor deve enviar para \ndeterminar o tempo médio de comunicação entre o servidor e o cliente.\n");
    exit(1);
  }

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
	//local.sin_addr.s_addr = inet_addr(ip);
	local.sin_addr.s_addr = INADDR_ANY;
	memset((&local.sin_zero), '\0', 8);

	bind(meuSocket, (struct sockaddr *)&local, sizeof(local));
	listen(meuSocket, BACKLOG);

  char message[9] = "Enviando\n";

  novoSocket = accept(meuSocket, (struct sockaddr *)&remote, &tamanho);
  if(novoSocket == 1){
    perror("\nErro ao aceitar conexao do usuario.\n");
    exit(1);
  }
  else {
    printf("Conectado pelo cliente: \nIP: %s", inet_ntoa(remote.sin_addr));
    printf("\nPorta: %d\n", ntohs(remote.sin_port));
    for (int i = 0; i < numeroSend; i++) {
      if(write(novoSocket, message ,strlen(message)) < 0){
      // if( send(novoSocket , novoSocket , strlen(message) , 0) < 0){
          printf("Erro em send()\n");
          close(novoSocket);
          close(meuSocket);
          exit(-1);
      } else {
        printf("%d\n",i );
        // if ((numbytes = read(novoSocket,buf,strlen(MAXDATASIZE))) == -1){
        if ((numbytes=recv(novoSocket, buf, MAXDATASIZE, 0)) == -1){
          perror("recv");
          exit(1);
        }
        else {
          buf[numbytes] = '\0';
          printf("Recebido: %s \n", buf);
        }
      }
    }
  }
  close(novoSocket);
	close(meuSocket);

	return 0;
}
