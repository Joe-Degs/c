struct Node {
    int key;
    struct Node* next;
};

struct List {
    struct Node* head;
    struct Node* tail;
}

void Add(struct List* list, int data) {
    temp = (struct Node*)malloc(sizeof(*node));
    temp->key = data;
    temp->next = list->tail;

    if (list->head == NULL) return;
}

void Traverse(struct List* list) {}

void Remove(struct List* list, int pos) {}
