struct node {
    void *data;
    struct node *next;
};

struct node *node_create(void *data, struct node *next);

void node_destroy(struct node *node);

void node_attach(struct node *from, struct node *to);

struct node *node_index(struct node *head_node, int i);

void node_test();
