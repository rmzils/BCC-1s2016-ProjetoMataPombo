#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>


double tempo(){
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return tv.tv_sec + tv.tv_usec/1e6;
}


int main(int arg, char *argv[]){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;

	// Criar scoket / AF_NET = / SOCK_STREAM = / ;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0){
		printf("Nao foi possivel criar o socket.\n");
		return 1;
	}

	// Preparar o sockaddr_in / ;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	// Fazendo do socket para torna-lo um servidor;
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Falha no bind do server\n");
		return 1;
	}

	printf("Bind feito\n");

	printf("Aguardando pedidos de conexao...\n");

	// Aberto a tentativas de conexao de clientes;

	listen(socket_desc, 3);

	// Aceitar conexao;

	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

	if(new_socket < 0){
		printf("Falha na aceitacao\n");
		return 1;
	}

	printf("Cliente conectado\n");

	double media_tempo = 0, start_t;
	int i = 0, read_size;
	char message[2000];
	time_t resp_t;

	// Esperando entrada de texto do terminal;

	for(fgets(message, 2000, stdin); strncmp(message, "close", 5) != 0; fgets(message, 2000, stdin), i++){
		char client_message[2000];

<<<<<<< HEAD
		time(&resp_t);
		start_t = tempo();

		printf("Enviando mensagem...\n");

		// Enviando mensagem ao cliente;
=======
		time(&start_t);
		printf("Envio de mensagem: ");
		star = tempo();
>>>>>>> origin/master

		if(send(new_socket, message, strlen(message), 0) < 0){
			printf("Erro no envio da mensagem\n");
			break;
		}

		// "verificar se o time() retorna uma variavel alocada";
		// Espera o ack do cliente durante 2 segundos, enquanto isso continua tentando mandar a mensagem;
		while(difftime(time(NULL), resp_t) < 2.0 && (read_size = recv(new_socket, client_message, 2000, 0)) < 0){
			printf("tempo de espera: %lf\n", difftime(time(NULL), resp_t));

			// Verifica se houve erro no envio da mensagem;
			if(send(new_socket, message, strlen(message), 0) < 0){
				printf("Erro no envio da mensagem\n");
				break;
			}
		}

		if(read_size == 0){
			printf("Cliente desconectado\n");
			break;
		}

		// Verifica se o erro foi no recv da mensagem;
		if(read_size < 0){
        	printf("Falha no recv\n");
        	break;
    	}

    	// Verifica se o erro foi no tempo de espera pelo ack;
    	if(difftime(time(NULL), start_t) > 2.0){
    		printf("Tempo de resposta excedido...\n");
    		break;
    	}
<<<<<<< HEAD

    	printf("Acknoledge recebido...\n");
=======
			printf("Recebeu ACK: ");
			fim = tempo();
			diferenca = fim - star;
			printf("Diferenca: %lf\t \n",diferenca);
			media_tempo += diferenca;
    	// media_tempo += difftime(time(NULL), start_t);

    	// printf("%s\n", client_message);
>>>>>>> origin/master

    	// Calcula diferenca de tempo entre o primeiro send e o tempo em que o ack foi recebido;
		media_tempo += tempo() - start_t;
		printf("Tempo de resposta: %lf\t \n",tempo() - start_t);
	}

	// Se houve pelo menos 1 resposta, imprime a media do tempo;
	if(i > 0){
		media_tempo /= i;
		printf("media de tempo: %lf\n", media_tempo);
	}

	close(socket_desc);

	return 0;
}