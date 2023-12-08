#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_REQUEST_SIZE 1024

void handle_get(int client_socket, const char *request) {
        const char *response = "HTTP/1.1 200 OK\r\n\r\nHello, this is a GET request!\n";
        send(client_socket, response, strlen(response), 0);
}

void handle_post(int client_socket, const char *request) {
        const char *response = "HTTP/1.1 200 OK\r\n\r\nHello, this is a POST request!\n";
        send(client_socket, response, strlen(response), 0);
}

void handle_cgi(int client_socket, const char *request) {
	const char *response = "HTTP/1.1 200 OK\r\n\r\nContent-type: text/html\r\n\r\n<html><body><h2>CGI Script Example</h2></body></html>\n";
	send(client_socket, response, strlen(response), 0);
}

void handle_request(int client_socket, const char *request) {
	if (strncmp(request, "GET", 3) == 0) {
	         handle_get(client_socket, request);
	} else if (strncmp(request, "POST", 4) == 0) {
	         handle_post(client_socket, request);
	} else if (strncmp(request, "CGI", 3) == 0) {
	         handle_cgi(client_socket, request);
	} else {
	       const char *response = "HTTP/1.1 400 Bad Request\r\n\r\nInvalid request";
	       send(client_socket, response, strlen(response), 0);
	}
}

void start_server() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char request[MAX_REQUEST_SIZE];

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	
	bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	listen(server_socket, 5);
	printf("Server listening on port %d...\n", PORT);
	
	while (1) {

		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		recv(client_socket, request, MAX_REQUEST_SIZE, 0);
		handle_request(client_socket, request);
	
		close(client_socket);

	}
	close(server_socket);

}

int main() {
	    start_server();
	        return 0;
}

