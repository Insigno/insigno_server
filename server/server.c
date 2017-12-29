#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <search.h>
#include "BST.h"
#include "user_management.h"

int get_socket();
void setup_server_address(struct sockaddr_in *server);
void bind_socket_to_server(int socket_desc, struct sockaddr_in *server);
void setup_server();
void *handle_connection(void *client_sock);
void client_status(int read_size);


extern Node *root;

int main(int argc, char *argv[]) {
    int socket_desc; 
    setup_server(&socket_desc);

    pthread_t connection_thread;

    while(1) {
        int client_desc = accept(socket_desc, NULL, NULL); 
        pthread_create(&connection_thread, NULL, handle_connection, &client_desc);
        puts("Connection accepted");
    }

    return 0;
}

void setup_server(int *socket_desc) {
    struct sockaddr_in server;

    *socket_desc = get_socket();
    if(*socket_desc == -1) {
        printf("Could not create socket");
        exit(1);
    }

    puts("Socket created");
    setup_server_address(&server);
    bind_socket_to_server(*socket_desc, &server);

    listen(*socket_desc, 3);
    puts("Waiting for incoming connections...");
}

int get_socket(){
    return socket(AF_INET, SOCK_STREAM, 0);
}  

void setup_server_address(struct sockaddr_in *server){
    server->sin_family = AF_INET; 

    // INADDR_ANY is used to bind the socket to any IP-Address (only used for localhost)
    server->sin_addr.s_addr = INADDR_ANY; 

    // Set portnumber, uses htons to automatically choose correct endianess
    server->sin_port = htons(8888); 
}

void bind_socket_to_server(int socket_desc, struct sockaddr_in *server) {

    if(bind(socket_desc, (struct sockaddr *)server, sizeof(*server)) < 0) {
        perror("Bind failed, Error");
        exit(1);;
    }
    puts("bind done");
}

// MAIN STUFF GOES ON HERE
void *handle_connection(void *client_sock) {
    int read_size;
    char message[1000];
    int client_socket = *((int *)client_sock);
    int other_user_socket;
    User *user;
    User *other_user;

    user = add_user(client_socket);
    other_user = connect_to_user(client_socket);
    other_user_socket = other_user->file_descriptor;

    // Send meldinger til den andre brukeren
    while((read_size = read(client_socket, message, 1000)) > 0) {
        message[read_size] = '\0';
        write(other_user_socket, message , strlen(message));

		// Clear the message buffer
		memset(message, 0, 1000);
    }

    client_status(read_size);
    del_user(user);

    return 0;
}

void client_status(int read_size) {
    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("recv failed");
    }
}

