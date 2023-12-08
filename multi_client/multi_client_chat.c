
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 3490
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *message, int sender_socket) {
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
	        if (clients[i] != sender_socket) {
	             if (send(clients[i], message, strlen(message), 0) == -1) {
		                    perror("Failed to send message");
	             }
	        }
	}
	pthread_mutex_unlock(&mutex);
}

void *handle_client(void *arg) {
	int client_socket = *((int *)arg);
	char buffer[1024];

	while (1) {
	       int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
	       if (bytes_received <= 0) {
	               pthread_mutex_lock(&mutex);
	               for (int i = 0; i < client_count; i++) {
	                    if (clients[i] == client_socket) {
	                            memmove(&clients[i], &clients[i + 1], (client_count - i - 1) * sizeof(clients[0]));
	                            client_count--;
		                    break;
		             }
		       }
		       pthread_mutex_unlock(&mutex);
		       close(client_socket);
		       break;
		}

       	       buffer[bytes_received] = '\0';
	       broadcast(buffer, client_socket);
        }

	pthread_exit(NULL);
}

int main() {
	    int server_socket, client_socket;
	    struct sockaddr_in server_addr, client_addr;
	    socklen_t client_addr_len = sizeof(client_addr);
	    pthread_t tid;

	    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	            perror("Socket creation failed");
	            exit(EXIT_FAILURE);
	     }
            server_addr.sin_family = AF_INET;
 	    server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(PORT);

	    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
	                perror("Binding failed");
	                exit(EXIT_FAILURE);
	    }

	    if (listen(server_socket, MAX_CLIENTS) == -1) {
	  	        perror("Listening failed");
		        exit(EXIT_FAILURE);
	    }

	    printf("Server listening on port %d...\n", PORT);
            while (1) {
	        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {  			        perror("Acceptance failed");
       		                continue;
	        }
	        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	        pthread_mutex_lock(&mutex);
	        clients[client_count++] = client_socket;
	        pthread_mutex_unlock(&mutex);
                if (pthread_create(&tid, NULL, handle_client, &client_socket) != 0) {
    	        	    perror("Thread creation failed");
        	            break;
            	}
	    }
	    close(server_socket);
            return 0;
}

