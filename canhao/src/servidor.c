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

void *projetil_handler(void *p);

void finaliza_aviao(){
	//envia msg para cliente falando que derrubou a pomba
	exit(EXIT_FAILURE);

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
	// estilo da mensagem : posicao x posicao y posicao z velocidade tempo
	// exemplo de mensagem : 0 55000.0 56000.0 500.0 100.0 10.0

	aviao *pomba = malloc(sizeof(aviao));
	int aviao_detectado = 0;
	double tempo_p, tempo_a;
	double tempo_disparo;
	int projetil_count = 4;
	
	projetil *projeteis[4];

	for(int i = 0; i < 4; i++)
		projeteis[i] = NULL;

	// FLAGS

	int projetil_calculado = 0;
	int espera_proximo_ponto = 1;
	int intervalo = 0.0;

	while(1){
		char client_message[2000];

		int read_size = recv(new_socket, client_message, 2000, 0);

		if(read_size >= 0){
			int tipo_mensagem;

			sscanf(client_message, "%d", &tipo_mensagem);

			//printf("tipo_mensagem: %d\n", tipo_mensagem);

			if(tipo_mensagem == 0){
				//printf("primeira vez\n");

				sscanf(client_message,"%d %lf %lf %lf %lf %lf", &tipo_mensagem, &pomba->pos_atux, &pomba->pos_atuy, &pomba->pos_atuz, &pomba->velocidade, &pomba->tempo_ini);
				pomba->velocidade = 0.0;
				projetil_count = 4;

				aviao_detectado = 1;
				projetil_calculado = 0;
				espera_proximo_ponto = 1;
			}

			if(tipo_mensagem == 1){
				//printf("segunda vez ou +\n");

				pomba->pos_antx = pomba->pos_atux;
				pomba->pos_anty = pomba->pos_atuy;
				pomba->pos_antz = pomba->pos_atuz;

				sscanf(client_message,"%d %lf %lf %lf %lf %lf", &tipo_mensagem, &pomba->pos_atux, &pomba->pos_atuy, &pomba->pos_atuz, &pomba->velocidade, &pomba->tempo_atu);

				//printf("aviao: %lf %lf %lf %lf\n", pomba->pos_atux, pomba->pos_atuy, pomba->pos_atuz, pomba->tempo_atu);

				//calcula_velocidade(pomba);

				//printf("velocidade: %lf\n", pomba->velocidade);

				if(pomba->pos_antz != pomba->pos_atuz){
					projetil_calculado = 0;
					espera_proximo_ponto = 1;
				}
				else
					espera_proximo_ponto = 0;
			}
		}

		if(projetil_count > 0 && aviao_detectado && !espera_proximo_ponto){
			// calcula tempo que vai demorar pra pomba chegar no ponto e tempo que o projetil vai demorar

			if(!projetil_calculado && pomba->pos_atuz == 200.0 && tempo() > intervalo){

				double tempo_inter = pomba->tempo_atu + 0.5;
				double ponto[3];

				while(1){
					ponto_alvo(ponto, pomba, tempo_inter);

					//printf("x:%lf y:%lf z:%lf\n", ponto[0], ponto[1], ponto[2]);

					//calcula angulos

					if(projeteis[projetil_count - 1] == NULL){
						projeteis[projetil_count - 1] = projetil_aloca(50000, 50000, 0, 0.0, 0.0, 1175);
					}

					double angulo_azemuth = calcula_azemuth(projeteis[projetil_count - 1], pomba);
					double angulo_z = calcula_angulo_disparo(projeteis[projetil_count - 1], ponto, tempo_inter);

					//printf("azemuth: %lf\n", angulo_azemuth);
					//printf("angulo z: %lf\n", angulo_z);

					projetil_atualiza(projeteis[projetil_count - 1], angulo_z, angulo_azemuth);

					tempo_p = tempo_projetil(projeteis[projetil_count - 1], ponto);
					tempo_a = tempo_aviao(ponto, pomba);

					//printf("tempo_projetil: %lf, tempo_aviao: %lf\n", tempo_p, tempo_a);


					if(distancia_ponto(projeteis[projetil_count - 1], ponto) > 4000.0 || tempo_p > tempo_inter)
						tempo_inter += 0.5;
					else
						break;
				}

				printf("ponto: x:%lf y:%lf z:%lf\n", ponto[0], ponto[1], ponto[2]);

				tempo_disparo = tempo() + tempo_a - tempo_p;

				printf("tempo_disparo: %lf\n", tempo_disparo);

				projetil_calculado = 1;
			}

			else if(projetil_calculado && pomba->pos_atuz == 200.0 && tempo() >= tempo_disparo && projetil_count > 3){
				//dispara, ou seja, cria thread do projetil

				printf("vai disparar projetil: %d\n", projetil_count);

				projetil_dispara(projeteis[projetil_count - 1], tempo());

				printf("disparou\n");

				intervalo = tempo() + 2.0;

				/*pthread_t sniffer_thread;

				if(pthread_create(&sniffer_thread, NULL, projetil_handler, projeteis[projetil_count - 1]) < 0){
	            	perror("could not create thread");
	            	return 1;
	        	}*/

				projetil_calculado = 0;
				projetil_count--;
			}
		}

		//verifica colisao

		//system("clear");

		for(int i = 0; i < 4; i++){
			if(projeteis[i] != NULL && projeteis[i]->tempo_de_disparo > 0){
				//printf("calculando colisao\n");

				atualizar_posicao(projeteis[i], tempo());

				printf("projetil %d:\n", i+1);

				if(colisao(projeteis[i], pomba, tempo())){
					printf("colidiu\n");
					// desaloca projeteis e manda msg para o cliente de aviao abatido

					for(int j = 0; j < 4; j++){
						printf("vai desalocar projetil %d\n", j);
						if(projeteis[j] != NULL){
							free(projeteis[j]);
							projeteis[j] = NULL;

							printf("desalocou projetil\n");
						}
					}

					finaliza_aviao();
				}

				if(projeteis[i]->z < 0){
					free(projeteis[i]);
					projeteis[i] = NULL;
				}
			}
		}
	}

	close(socket_desc);

	return 0;
}

/*void *projetil_handler(void *p){
	projetil proj = *(projetil*)p;

	while(proj.z >= 0.0)
		atualizar_posicao(&proj, tempo());

	return 0;
}*/