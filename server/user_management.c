#include "BST.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

User *add_user(int client_socket) {
    char id[10];
    // Get id
    read(client_socket, id, 10);

    User *user = malloc(sizeof(User));
    strcpy(user->id, id);
    user->file_descriptor = client_socket;
    insert(root, *user);

    return user;
}

void del_user(User *user) {
    delete(root, *user);
}

User *connect_to_user(int client_socket) {
    //TODO: Send list of currently online clients
    //do som sort of while loop that only continues when client has choosen an online client

    char connect_id[10];
    
    // Get ID of user to connect to
    read(client_socket, connect_id, sizeof(connect_id));

    Node *node = malloc(sizeof(Node));

    // TODO: should return User struct
    node = get(root, connect_id);

    if(node != NULL) {
        puts("FOUND USER");
        return &(node->user);
    }

    return NULL;
}


