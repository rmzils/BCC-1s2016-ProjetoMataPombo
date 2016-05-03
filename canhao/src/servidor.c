#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "calculos.h"
#include "projetil.h"

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

	double media_tempo = 0, start_t;
	int i = 0, read_size;
	char message[2000];
	time_t resp_t;

	// Aguardando mensagem do cliente

	// codigos : aviao detectado 				 = 0
	//			 atualizacao da posicao do aviao = 1
	//			 
	// estilo da mensagem : posicao x posicao y posicao z tempo
	// exemplo de mensagem : 0 55000.0 56000.0 500.0 10.0

	projetil *p = NULL;
	aviao *pomba = malloc(sizeof(aviao));
	int aviao_detectado = 0;
	int tempo_disparo;
	int projetil_count = 0;

	double ponto_alvo[2];
	
	projetil *projeteis[4];

	for(int i = 0; i < 4; i++)
		projeteis[i] = NULL;

	// FLAGS

	int projetil_calculado = 0;
	int espera_proximo_ponto = 1;

	time(&resp_t);

	while(1){
		char client_message[2000];

		int read_size = recv(new_socket, client_message, 2000, 0);

		if(read_size >= 0){
			int tipo_mensagem;

			sscanf(client_message, "%d", tipo_mensagem);

			if(tipo_mensagem == "0"){
				sscanf(client_message,"%lf %lf %lf %lf", pomba->pos_antx, pomba->pos_anty, pomba->pos_antz, pomba->tempo_ini);
				pomba->velocidade = 0.0;
				disparos = 4;

				aviao_detectado = 1;
				projetil_calculado = 0;
				espera_proximo_ponto = 1;
			}

			if(tipo_mensagem == "1"){
				sscanf(client_message,"%lf %lf %lf %lf", pomba->pos_atux, pomba->pos_atuy, pomba->pos_atuz, pomba->tempo_atu);

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

		if(disparos > 0 && aviao_detectado && !espera_proximo_ponto){
			// calcula tempo que vai demorar pra pomba chegar no ponto e tempo que o projetil vai demorar

			if(!projetil_calculado){
				double tempo_inter = p->tempo_atu + 1.0;

				while(1){


					ponto_alvo(ponto_alvo, pomba, tempo_inter);

					//calcula angulos

					double angulo_azimuth;
					double angulo_z;

					projetil[projetil_count] = aloca_projetil(50000, 50000, 0, angulo_z, angulo_azimuth, 1175);

					double tempo_projetil = tempo_projetil(projetil[projetil_count], ponto_alvo);

					if(tempo_projetil > tempo_inter)
						tempo_inter += 1.0;
					else
						break;
				}

				tempo_disparo = tempo() + tempo_inter - p->tempo_atu;

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
				if(colisao(projeteis[i], aviao, tempo())) {
					// desaloca projeteis e manda msg para o cliente de aviao abatido

					finaliza_aviao(projeteis);
				}

				else{
					//verifica se projetil chegou no chao

					if(projetil_faio(projeteis[i], tempo()){
						free(projeteis[i]);
						projeteis[i] = NULL;
					}
				}
			}
		}
	}

	// Esperando entrada de texto do terminal;

	/*for(fgets(message, 2000, stdin); strncmp(message, "close", 5) != 0; fgets(message, 2000, stdin), i++){
		char client_message[2000];

		time(&resp_t);
		start_t = tempo();

		printf("Enviando mensagem...\n");

		// Enviando mensagem ao cliente;
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

    	printf("Acknowledgment recebido.\n");

    	// Calcula diferenca de tempo entre o primeiro send e o tempo em que o ack foi recebido;
		media_tempo += tempo() - start_t;
		printf("Tempo de resposta: %lf\t \n",tempo() - start_t);

		//strcpy(message,"");
		//strcpy(client_message,"");
	}

	// Se houve pelo menos 1 resposta, imprime a media do tempo;
	if(i > 0){
		media_tempo /= i;
		printf("media de tempo: %lf\n", media_tempo);
	}*/

	close(socket_desc);

	return 0;
}
