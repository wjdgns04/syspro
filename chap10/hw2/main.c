#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

void addq(struct node *head, struct node *tail, int data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;

    if (head->next == NULL) {
        head->next = new_node;
        tail->next = new_node;
    } else {
        tail->next->next = new_node;
        tail->next = new_node;
    }
}

int delete(struct node *head, struct node *tail) {
    struct node *temp = head->next;
    int val;

    if (temp == NULL) {
        return -1;
    }
    val = temp->data;
    head->next = temp->next;
    if (head->next == NULL) {
        tail->next = NULL;
    }
    free(temp);
    return val;
}

void print_queue(struct node *head) {
    struct node *curr = head->next;
    printf("Print queue\n");
    while (curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}

int main() {
    struct node head = {0, NULL};
    struct node tail = {0, NULL};
    int val;
    char buf[64];

    while (scanf("%63s", buf) == 1) {
        char *endptr;
        long num = strtol(buf, &endptr, 10);

        if (*endptr == '\0' && buf[0] != '\0' && num > 0) {
            addq(&head, &tail, (int)num);
        } else {
            break;
        }
    }

    print_queue(&head);

    while ((val = delete(&head, &tail)) != -1) {
        (void)val;
    }
    return 0;
}
