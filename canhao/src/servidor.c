#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "projetil.h"
#include "calculos.h"

double tempo(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec/1e6;
}

void finaliza_aviao(){
	//envia msg para cliente falando que derrubou a pomba
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
	server.sin_addr.s_addr = INADDR_ANY; /* IP do Host */
	server.sin_family = AF_INET; /* Familia do endereco usando: (ARPA INTERNET PROTOCOLS) */
	server.sin_port = htons(8888); /* Numero da porta */

	// Fazendo bind do socket para torna-lo um servidor;
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

	time_t resp_t;
	time(&resp_t);

	// Aguardando mensagem do cliente

	// codigos : aviao detectado 				 = 0
	//			 atualizacao da posicao do aviao = 1
	//			 
	// estilo da mensagem : posicao x posicao y posicao z tempo
	// exemplo de mensagem : 0 55000.0 56000.0 500.0 10.0

	aviao *pomba = malloc(sizeof(aviao));
	int aviao_detectado = 0;
	int tempo_disparo;
	int projetil_count = 4;
	
	projetil *projeteis[4];

	for(int i = 0; i < 4; i++)
		projeteis[i] = NULL;

	// FLAGS

	int projetil_calculado = 0;
	int espera_proximo_ponto = 1;

	while(1){
		char client_message[2000];

		int read_size = recv(new_socket, client_message, 2000, 0);

		if(read_size >= 0){
			int tipo_mensagem;

			sscanf(client_message, "%d", &tipo_mensagem);

			if(tipo_mensagem == 0){
				sscanf(client_message,"%lf %lf %lf %lf", &pomba->pos_antx, &pomba->pos_anty, &pomba->pos_antz, &pomba->tempo_ini);
				pomba->velocidade = 0.0;
				projetil_count = 4;

				aviao_detectado = 1;
				projetil_calculado = 0;
				espera_proximo_ponto = 1;
			}

			if(tipo_mensagem == 1){
				sscanf(client_message,"%lf %lf %lf %lf", &pomba->pos_atux, &pomba->pos_atuy, &pomba->pos_atuz, &pomba->tempo_atu);

				pomba->velocidade = calcula_velocidade(pomba);

				if(pomba->pos_antz != pomba->pos_atuz){
					projetil_calculado = 0;
					espera_proximo_ponto = 1;
				}
				else
					espera_proximo_ponto = 0;

				pomba->pos_antx = pomba->pos_atux;
				pomba->pos_anty = pomba->pos_atuy;
				pomba->pos_antz = pomba->pos_atuz;
			}
		}

		if(projetil_count > 0 && aviao_detectado && !espera_proximo_ponto){
			// calcula tempo que vai demorar pra pomba chegar no ponto e tempo que o projetil vai demorar

			if(!projetil_calculado){
				double tempo_inter = pomba->tempo_atu + 0.5;
				double ponto[3];
				double tempo_p;

				while(1){
					ponto_alvo(ponto, pomba, tempo_inter);

					//calcula angulos

					double angulo_azimuth;
					double angulo_z = calcula_angulo_disparo(projeteis[projetil_count], ponto);

					if(projeteis[projetil_count] != NULL)
						projeteis[projetil_count] = projetil_aloca(50000, 50000, 0, angulo_z, angulo_azimuth, 1175);
					else
						projetil_atualiza(projeteis[projetil_count], angulo_z, angulo_azimuth);

					tempo_p = tempo_projetil(projeteis[projetil_count], ponto);

					if(tempo_p > tempo_inter)
						tempo_inter += 0.6;
					else
						break;
				}

				tempo_disparo = tempo() + tempo_inter - pomba->tempo_atu;

				projetil_calculado = 1;
			}

			else if(tempo() >= tempo_disparo){
				//dispara, ou seja, cria thread do projetil

				projetil_calculado = 0;
				projetil_count--;
			}
		}

		//verifica colisao
		for(int i = 0; i < 4; i++){
			if(projeteis[i] != NULL){
				if(colisao(projeteis[i], pomba, tempo())) {
					// desaloca projeteis e manda msg para o cliente de aviao abatido

					for(int i = 0; i < 4; i++)
						if(projeteis[i] != NULL)
							free(projeteis[i]);

					finaliza_aviao();
				}

				else{
					//verifica se projetil chegou no chao

					if(projeteis[i]->z < 0){
						free(projeteis[i]);
						projeteis[i] = NULL;
					}
				}
			}
		}
	}

	close(socket_desc);

	return 0;
}
