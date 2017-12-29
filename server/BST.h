#include <stddef.h>

typedef struct Node Node;
typedef struct User User;

Node *root;

struct User {
    char id[10];
    int file_descriptor;
};


struct Node { 
    char *id;
    User user;
    Node *left;
    Node *right;
};

Node *insert(Node *x, User user);

Node *get(Node *x, char *id);

Node *delete(Node *x, User user_to_delete);

Node *min(Node *x);

Node *deleteMin(Node *x);

void print(Node *x);


