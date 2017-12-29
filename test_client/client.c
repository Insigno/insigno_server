
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

void *writeToSocket();
void *readFromSocket();

int socket_FD;
struct sockaddr_in server;
char server_reply[2000];


int main(int argc, char *argv[]) {

    // Create socket;
    socket_FD = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_FD == -1) {
        printf("Unable to create socket");
    }
    puts("Socket created!!");
    puts("Enter server ip: ");
    char server_ip[27]; 
    scanf("%s", server_ip);
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    
    // connect to remote server
    if(connect(socket_FD, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    char id[10];
    puts("Enter ID:");
    scanf("%s", id);
    if(send(socket_FD, id, 10, 0) < 0) {
        puts("Send failed");
    }

    puts("Enter friends ID: ");
    char other_id[10];;
    scanf("%s", other_id);
    if(send(socket_FD, other_id, 10, 0) < 0) {
        puts("Send failed");
    }

    pthread_t readThread;
    pthread_t writeThread;

    pthread_create(&readThread, NULL, readFromSocket, NULL);
    pthread_create(&writeThread, NULL, writeToSocket, NULL);

    pthread_join(readThread, NULL);

    return 0;
}

void *writeToSocket() {
    while(1) {
        char message[1000] = "";
        printf("Enter message : \n");
        fgets(message, 1000, stdin);

        // send message
        if( send(socket_FD, message, strlen(message), 0) < 0) {
            puts("send failed");
        }

        fflush(stdout);
        usleep(100);
    }
}


void *readFromSocket() {
    int read_size;
    char message[1000];
    while(1) {
        if(recv(socket_FD, message, 1000, 0) > 0) {
            puts("Server response: ");
            puts(message);
            memset(message, 0, 1000);
        }
    }
}

