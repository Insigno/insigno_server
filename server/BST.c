#include "BST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Node *root;

Node *insert(Node *x, User user) {
    if(x == NULL) {
        x = malloc(sizeof(Node));
        x->user = user;
        x->left = NULL;
        x->right = NULL;
    }
    else if(strcmp(user.id, x->user.id) < 0) {
        x->left = insert(x->left, user);
    }
    else {
        x->right = insert(x->right, user);
    }

    root = x;
    return x;
}

Node *get(Node *x, char *id) {
    if(x == NULL) return NULL;

    int compare = strcmp( id, x->user.id);
    if (compare < 0) return get(x->left, id);
    else if (compare > 0) return get(x->right, id);
    else return x;
}

Node *delete(Node *x, User user_to_delete) {
    if(x == NULL) return NULL;

    int compare = strcmp(user_to_delete.id, x->user.id);

    if(compare < 0) {
        x->left = delete(x->left, user_to_delete);
    }
    else if(compare > 0) {
        x->right = delete(x->right, user_to_delete);
    }
    else {
        if(x->right == NULL) return x->left;
        if(x->left == NULL) return x->right;
        Node *t = x;
        x = min(t->right);
        x->right = deleteMin(t->right);
        x->left = t->left;
    }

    return x;
}

Node *min(Node *x) {
    if (x->left == NULL) return x;
    else return min(x->left);
}

Node *deleteMin(Node *x) {
    if(x->left == NULL) return x->right;
    x->left = deleteMin(x->left);
    return x;

}

void print(Node *x) {
    if(x == NULL) {
        return;
    }

    print(x->left);
    puts(x->user.id);
    print(x->right);
}

void test() {
    User user;
    user.file_descriptor = 10;
    strcpy(user.id, "HELLO");

    User user1;
    user1.file_descriptor = 12;
    strcpy(user1.id, "lars");

    User user2; 
    user2.file_descriptor = 13;
    strcpy(user2.id, "geir");

    insert(root, user);
    insert(root, user1);
    insert(root, user2);

    print(root);
    delete(root, user1);
    print(root);

    Node *hey = get(root, user2.id);

    puts(hey->user.id);
}





