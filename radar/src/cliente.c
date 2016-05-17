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
	int socket_desc;
	struct sockaddr_in server;

    // Criar scoket / AF_NET = / SOCK_STREAM = / ;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
		printf("Nao foi possivel criar o socket.\n");

    // Preparar o sockaddr_in /

	server.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.34.214"); /* IP do Host */
	server.sin_family = AF_INET; /* Familia do endereco usando: (ARPA INTERNET PROTOCOLS) */
	server.sin_port = htons(8888); /* Numero da porta */

	printf("Tentando conectar...\n");

    // Tentando conectar com o servidor;
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Erro na conexao\n");
		return 1;
	}

	printf("Conectado\n");

	// close(socket_desc);

	aviao pombo;
	movimentoAviao movimentoPombo;
	pombo.primeiraVez = '0';

	//angulo e posição do pombo
	randomPosicaoAviao(&pombo, &movimentoPombo);

	double tempoNovo, tempoAntigo;
	tempoAntigo = tempo();

	char *strMensagem;

	srand(time(NULL));

	while (1) {
		tempoNovo = tempo();
		if ((tempoNovo - tempoAntigo) > TOLERANCIA ) {

			if (pombo.tempoDeVoo > pombo.tempoMudancaTrajetoria) {
				direcaoAoAlvo(&pombo, &movimentoPombo);
				printf("ACABOOOU!\n");
			}
			else {
				if( pombo.atacou == 1 ){
					printf("ATACOU! Perdemos conexão com o servidor! :/\n");
					return 0;
				}
				system("clear");
				pombo.tempoDeVoo += tempoNovo - tempoAntigo;
				printf("%.5f\n", pombo.tempoDeVoo );
				tempoAntigo = tempoNovo;
				tempoNovo = tempo();
				strMensagem = strInfPombo( &pombo, &movimentoPombo, tempoNovo );
				printf("Mensagem: %s\n\n\n", strMensagem);
				free(strMensagem);
				calculandoProximoPonto(&pombo, &movimentoPombo);

				printf("############################################################\n");
				printf("Ponto Avião: %.3f - %.3f\n", movimentoPombo.pontoHipotenusa[0], movimentoPombo.pontoHipotenusa[1] );
				printf("tempo mudanca: %f\n",pombo.tempoMudancaTrajetoria);
				printf("pombo.anguloDirecao: %f\n",pombo.anguloDirecao );
				printf("velc: %.2f - tipo traj: %d\n", pombo.velocidade, pombo.tipoTrajetoria);;
				printf("############################################################\n\n");

				write(socket_desc, strMensagem, strlen(strMensagem));
				pombo.primeiraVez = '1';
			}
		}
	}

	return 0;
}
