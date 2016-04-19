#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
//só tipos:
#include "definicoes.h"
#include "aviao.h"
#include "movimentoaviao.h"
//metodos:
#include "estatistica.h"



double tempo(){
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return tv.tv_sec + tv.tv_usec/1e6;
}

int main(int argc, char *argv[]){

	aviao pombo;
	movimentoAviao movimentoPombo;

	//angulo e posição do pombo
	randomPosicaoAviao(&pombo, &movimentoPombo);

	printf("tempo mudanca: %f\n",pombo.tempoMudancaTrajetoria);
	printf("pombo.anguloDirecao: %f\n",pombo.anguloDirecao );
	printf("posicao x:%.3f - y:%.3f - z:%.3f\n", pombo.posX, pombo.posY, pombo.posZ );
	printf("velc: %.2f - tipo traj: %d\n", pombo.velocidade, pombo.tipoTrajetoria);;

	double tempoNovo, tempoAntigo;
	tempoAntigo = tempo();

	while (1) {
		tempoNovo = tempo();
		if ((tempoNovo - tempoAntigo) > 0.5) {

			if (pombo.tempoDeVoo > pombo.tempoMudancaTrajetoria) {

				direcaoAoAlvo(&pombo, &movimentoPombo);
				printf("ACABOOOU!\n");
			}
			else {
				pombo.tempoDeVoo += tempoNovo - tempoAntigo;
				printf("%.5f\n", pombo.tempoDeVoo );
				tempoAntigo = tempoNovo;
				tempoNovo = tempo();

				calculandoProximoPonto(&pombo, &movimentoPombo);

				printf("############################################################\n");

				printf("pontoInicialAviao: %.3f - %.3f\n", movimentoPombo.pontoInicialAviao[0], movimentoPombo.pontoInicialAviao[1] );
				printf("pontoCentroRadar: %.3f - %.3f\n", movimentoPombo.pontoCentroRadar[0], movimentoPombo.pontoCentroRadar[1] );
				printf("pontoCatetos: %.3f - %.3f\n", movimentoPombo.pontoCatetos[0], movimentoPombo.pontoCatetos[1] );
				printf("FINALMETE: pontoHipotenusa: %.3f - %.3f\n", movimentoPombo.pontoHipotenusa[0], movimentoPombo.pontoHipotenusa[1] );

				printf("tamanhoHipotenusa: %.3f \n", movimentoPombo.tamanhoHipotenusa );
				printf("tamanhoCatetoAdjacente: %.3f\n", movimentoPombo.tamanhoCatetoAdjacente);
				printf("tamanhoCatetoOposto: %.3f \n", movimentoPombo.tamanhoCatetoOposto );

				printf("############################################################\n\n");
			}
		}
	}

	// while (1) {
	// 	if (difftime(time(NULL), pombo.tempoDeVoo) != pombo.tempoMudancaTrajetoria) {
	//
	// 		if (difftime(time(NULL), pombo.tempoDeVoo) > aux) {
	// 			calculandoProximoPonto(&pombo, &movimentoPombo);
	//
	// 			printf("############################################################\n");
	//
	// 			printf("pontoInicialAviao: %.3f - %.3f\n", movimentoPombo.pontoInicialAviao[0], movimentoPombo.pontoInicialAviao[1] );
	// 			printf("pontoCentroRadar: %.3f - %.3f\n", movimentoPombo.pontoCentroRadar[0], movimentoPombo.pontoCentroRadar[1] );
	// 			printf("pontoCatetos: %.3f - %.3f\n", movimentoPombo.pontoCatetos[0], movimentoPombo.pontoCatetos[1] );
	// 			printf("FINALMETE: pontoHipotenusa: %.3f - %.3f\n", movimentoPombo.pontoHipotenusa[0], movimentoPombo.pontoHipotenusa[1] );
	//
	// 			printf("tamanhoHipotenusa: %.3f \n", movimentoPombo.tamanhoHipotenusa );
	// 			printf("tamanhoCatetoAdjacente: %.3f\n", movimentoPombo.tamanhoCatetoAdjacente);
	// 			printf("tamanhoCatetoOposto: %.3f \n", movimentoPombo.tamanhoCatetoOposto );
	//
	// 			printf("############################################################\n\n");
	//
	// 			aux = difftime(time(NULL), pombo.tempoDeVoo);
	// 		}
	// 	}
	// }

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
