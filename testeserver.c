#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

void *connection_handler(void *socket_desc);

double tempo(){
 struct timeval tv;
 char buffer[30];
 time_t curtime;

 gettimeofday(&tv, NULL);

 curtime=tv.tv_sec;
 strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
 printf("%s%d\n",buffer,tv.tv_usec);

 printf("segundos: %lf\t \n", tv.tv_sec + tv.tv_usec/1e6 );

 return tv.tv_sec + tv.tv_usec/1e6;
}


int main(int arg, char *argv[]){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	double star, fim, diferenca;

	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0){
		printf("Could not create socket\n");
		return 1;
	}

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Bind failed\n");
		return 1;
	}

	printf("Bind done\n");

	printf("Waiting for incoming connections...\n");

	listen(socket_desc, 3);

	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

	if(new_socket < 0){
		printf("accept failed\n");
		return 1;
	}

	printf("Connection accepted\n");

	/*pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = new_socket;

	if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0){
        printf("could not create thread\n");
        return 1;
    }

    //Now join the thread , so that we dont terminate before the thread
    //pthread_join( sniffer_thread , NULL);
    printf("Handler assigned\n");

    if(new_socket < 0){
		printf("accept failed\n");
		return 1;
	}*/

	double media_tempo = 0;
	int i = 0, read_size;
	char message[2000];

	time_t start_t, end_t;
	struct tm* formatted_time;

	for(fgets(message, 2000, stdin); strncmp(message, "close", 5) != 0; fgets(message, 2000, stdin), i++){
		char client_message[2000];

		time(&start_t);
		star = tempo();

		if(send(new_socket, message, strlen(message), 0) < 0){
			printf("erro no envio da mensagem\n");
			return 1;
		}

		while(difftime(time(NULL), start_t) < 2.0 && (read_size = recv(new_socket, client_message, 2000, 0)) < 0){
			if(send(new_socket, message, strlen(message), 0) < 0){
				printf("erro no envio da mensagem\n");
				return 1;
			}
		}

		if(read_size == -1){
        	printf("recv failed\n");
        	return 1;
    	}

    	if(difftime(time(NULL), start_t) > 2.0){
    		printf("tempo de resposta exedido..\n");
    		break;
    	}
			fim = tempo();
			diferenca = fim - star;
			printf("Diferenca: %lf\t \n",diferenca);
			media_tempo += diferenca;			
    	// media_tempo += difftime(time(NULL), start_t);

    	printf("%s\n", client_message);

	}

	if(i > 0){
		printf("total: %lf, total/i: %lf\n",media_tempo, media_tempo/i);
	}

	close(socket_desc);

	return 0;
}

/*void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
	int read_size;
    char *message, client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Its my duty to communicate with you\n";
    write(sock, message, strlen(message));

    while((read_size = recv(sock, client_message, 2000 , 0)) > 0){
        //Send the message back to client
        printf("%s", client_message);
    }

    printf("entrou aqui\n");

    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}*/
