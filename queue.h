#ifndef QUEUE_H
#define QUEUE_H

#include "simulation.h"

Node* enqueue(Node* head, Passenger p);

Node* dequeue(Node* head, Passenger* out);

int queueSize(Node* head);

#endif