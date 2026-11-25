#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

void push(struct node *top, int data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }
    new_node->data = data;
    new_node->next = top->next;
    top->next = new_node;
}

int pop(struct node *top) {
    struct node *temp = top->next;
    int val;

    if (temp == NULL) {
        return -1;
    }
    val = temp->data;
    top->next = temp->next;
    free(temp);
    return val;
}

void print_stack(struct node *top) {
    struct node *curr = top->next;
    printf("Print stack\n");
    while (curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}

int main() {
    struct node top = {0, NULL};
    double input;

    while (scanf("%lf", &input) == 1) {
        if (input > 0 && (int)input == input) {
            push(&top, (int)input);
        } else {
            break;
        }
    }

    print_stack(&top);

    while (top.next != NULL) {
        pop(&top);
    }
    return 0;
}
