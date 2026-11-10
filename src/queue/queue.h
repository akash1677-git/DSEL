#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 20
#define PROMPT_SIZE 1000

typedef struct {
    char prompts[QUEUE_SIZE][PROMPT_SIZE];
    int front;
    int rear;
    int count;
} Queue;

void initQueue(Queue *q);
int enqueue(Queue *q, const char *prompt);
int dequeue(Queue *q, char *prompt);

#endif
