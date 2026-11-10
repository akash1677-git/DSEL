
#include <stdio.h>
#include <string.h>
#include "queue.h"

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int enqueue(Queue *q, const char *prompt) {
    if (q->count == QUEUE_SIZE)
        return 0;

    q->rear = (q->rear + 1) % QUEUE_SIZE;

    snprintf(q->prompts[q->rear], PROMPT_SIZE, "%s", prompt);

    q->count++;
    return 1;
}

int dequeue(Queue *q, char *prompt) {
    if (q->count == 0)
        return 0;

    strcpy(prompt, q->prompts[q->front]);

    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;

    return 1;
}
