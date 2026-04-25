#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) {
        return NULL;
    }

    Node* head = (Node*)malloc(sizeof(Node));
    if (head == NULL) {
        return NULL;
    }
    head->id = 1;
    head->next = head;

    Node* tail = head;
    for (int i = 2; i <= n; ++i) {
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            free_list(head);
            return NULL;
        }
        node->id = i;
        node->next = head;
        tail->next = node;
        tail = node;
    }
    return head;
}

void free_list(Node* head) {
    if (head == NULL) {
        return;
    }

    Node* current = head->next;
    while (current != head) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(head);
}
