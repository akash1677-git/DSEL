#include "stack.h"

void initStack(Stack *s) {
    s->top = -1;
}

int isStackEmpty(Stack *s) {
    return s->top == -1;
}

int push(Stack *s, int value) {
    if (s->top == STACK_SIZE - 1)
        return 0;

    s->items[++s->top] = value;
    return 1;
}

int pop(Stack *s, int *value) {
    if (isStackEmpty(s))
        return 0;

    *value = s->items[s->top--];
    return 1;
}
