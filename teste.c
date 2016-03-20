#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

//void *connection_handler(void *socket_desc);

int main(int argc, char *argv[]){
	int socket_desc, *new_sock;
	struct sockaddr_in server;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
		printf("could not create socket.\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	printf("tentando conectar...\n");

	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("connect error\n");
		return 1;
	}

	printf("conectado\n");

	/*pthread_t sniffer_thread;
	new_sock = malloc(1);
    *new_sock = socket_desc;

	if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0){
        printf("could not create thread\n");
        return 1;
    }
         
    //Now join the thread , so that we dont terminate before the thread
    //pthread_join( sniffer_thread , NULL);
    printf("Handler assigned\n");*/

	// enviar mensagem

	/*char message[2000], server_reply[2000];

	while(strncmp((fgets(message, 2000, stdin)), "close", 5) != 0){

		if(send(socket_desc, message, strlen(message), 0) < 0){
			printf("send failed\n");
			break;
		}
	}*/

	int read_size;     
    char *message = "ack", server_message[2000];

	while((read_size = recv(socket_desc, server_message, 2000 , 0)) > 0){
        //Send the message back to server
        printf("%s", server_message);
        if(send(socket_desc, message, strlen(message), 0) < 0)
        	printf("erro no envio da mensagem\n");
    }

    if(read_size == 0){
        puts("Server disconnected");
        fflush(stdout);
    }

    else if(read_size == -1){
        perror("recv failed");
    }

	close(socket_desc);

	return 0;
}

/*void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
	int read_size;     
    char *message = "ack", server_message[2000];
     
    while((read_size = recv(sock, server_message, 2000 , 0)) > 0){
        //Send the message back to client
        printf("%s", server_message);
        if(send(sock, message, strlen(message), 0) < 0)
        	printf("erro no envio da mensagem\n");
    }

    printf("read_size: %d\n", read_size);
     
    if(read_size == 0){
        puts("Server disconnected");
        fflush(stdout);
    }

    else if(read_size == -1){
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}*/