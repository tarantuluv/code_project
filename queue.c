#include <stdlib.h>
#include "simulation.h"

Node* enqueue(Node* head, Passenger p)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->p = p;
    newNode->next = NULL;

    if (!head) return newNode;

    Node* temp = head;
    while (temp->next)
        temp = temp->next;

    temp->next = newNode;
    return head;
}

Node* dequeue(Node* head, Passenger* out)
{
    if (!head) 
    return NULL;

    Node* temp = head;
    *out = head->p;

    head = head->next;
    free(temp);

    return head;
}

int queueSize(Node* head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return count;
}