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
#include "projetil.h"
//metodos:
#include "estatistica.h"

int aviao_abatido = 0;
aviao pombo;
projetil *projeteis[4];

void *connection_handler(void *c);
void *projetil_handler(void *p);

double tempo(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec/1e6;
}

int main(int argc, char *argv[]){
	int socket_desc;
	struct sockaddr_in server;

	srand(time(NULL));

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

	pthread_t sniffer_thread;

	if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) &socket_desc) < 0){
	    perror("could not create thread");
	    return 1;
	}

	// close(socket_desc);

	movimentoAviao movimentoPombo;
	pombo.primeiraVez = '0';

	for(int i = 0; i < 4; i++)
		projeteis[i] = NULL;

	//angulo e posição do pombo
	randomPosicaoAviao(&pombo, &movimentoPombo);

	double tempoNovo, tempoAntigo;
	tempoAntigo = tempo();

	char strMensagem[100];

	while (!aviao_abatido) {
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
				//strMensagem = //strInfPombo( &pombo, &movimentoPombo, tempoNovo );
				sprintf(strMensagem, "%c %.7f %.7f %.7f %.7f %f\n", pombo.primeiraVez, pombo.posX, pombo.posY, pombo.posZ, pombo.velocidade, tempoNovo);
				printf("vel: %lf\n", pombo.velocidade);
				//free(strMensagem);
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

void *connection_handler(void *c){
	printf("criou thread --\n");
	int sock = *(int *)c;

	printf("criou thread\n");

	pthread_t sniffer_thread;

	if(pthread_create(&sniffer_thread, NULL, projetil_handler, NULL) < 0){
	    perror("could not create thread\n");
	    return 0;
	}

	while(1){
		char client_message[2000];

		//printf("carai1\n");

		int read_size = recv(sock, client_message, 2000, 0);

		//printf("carai2\n");

		if(read_size >= 0){
			int tipo_mensagem;

			sscanf(client_message, "%d", &tipo_mensagem);

			if(tipo_mensagem == 0){
				projetil *p = malloc(sizeof(projetil));

				sscanf(client_message,"%d %lf %lf %lf %lf", &tipo_mensagem, &p->velocidade_x, &p->velocidade_y, &p->velocidade_z, &p->tempo_de_disparo);
				//printf("%d %lf %lf %lf %lf\n", tipo_mensagem, p->velocidade_x, p->velocidade_y, p->velocidade_z, p->tempo_de_disparo);

				p->x_inicial = 50000.0;
				p->y_inicial = 50000.0;
				p->z_inicial = 0.0;
				p->aceleracao = -9.8;

				//printf("criou thread\n");
			}

			if(tipo_mensagem == 1){
				printf("derrubou\n");
				aviao_abatido = 1;
			}
		}

		if(read_size == -1){
			printf("carai\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

double colisao(projetil *p, aviao *pomba, double tempo){
	//atualiza posicao pomba

	double dist_x = p->x - pomba->posX;
	if(dist_x < 0)
		dist_x *= -1;

	double dist_y = p->y - pomba->posY;
	if(dist_y < 0)
		dist_y *= -1;

	double hipo_xy = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

	double dist_z = p->z - pomba->posZ;
	if(dist_z < 0)
		dist_z *= -1;

	return sqrt(pow(hipo_xy, 2) + pow(dist_z, 2));
}

void *projetil_handler(){
	while(1){
		for(int i = 0; i < 4; i++){
			projetil *p = projeteis[i];

			if(p != NULL){
				atualizar_posicao(p, tempo());

				double dist = colisao(p, &pombo, tempo());
			}
		}
	}

	return 0;
}
