#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "aviao.h"

int main(int argc, char *argv[]){
	int socket_desc;
	struct sockaddr_in server;

    // Criar scoket / AF_NET = / SOCK_STREAM = / ;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
		printf("Nao foi possivel criar o socket.\n");

    // Preparar o sockaddr_in /
	server.sin_addr.s_addr = inet_addr("169.254.203.108"); /* IP do Host */
	server.sin_family = AF_INET; /* Familia do endereco usando: (ARPA INTERNET PROTOCOLS) */
	server.sin_port = htons(8888); /* Numero da porta */

	printf("Tentando conectar...\n");

    // Tentando conectar com o servidor;
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Erro na conexao\n");
		return 1;
	}

	printf("Conectado\n");

	int read_size;
    char *message = "ack", server_message[2000];

    // Fica esperando uma mensagem do servidor;
	while((read_size = recv(socket_desc, server_message, 2000 , 0)) > 0){
        // Envia um ack para confirmacao do recebimento da mensagem;
        server_message[read_size]='\0';
        printf("%s", server_message);

        if(send(socket_desc, message, strlen(message), 0) < 0)
        	printf("Erro no envio da mensagem\n");
    }

		// se for 0 desconectou o servidor
    if(read_size == 0){
        puts("Servidor desconectado\n");
        fflush(stdout);
    }

    else if(read_size < 0){
        perror("Falha no recv\n");
    }

	close(socket_desc);

	return 0;
}