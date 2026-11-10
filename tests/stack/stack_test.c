
#include <stdio.h>

#include "stack.h"

int main() {
    Stack s;
    int value;
    int passed = 0;
    int total = 5;

    initStack(&s);

    printf("=================================\n");
    printf("        DSEL STACK TESTS\n");
    printf("=================================\n");

    // TEST 1: Push operation
    printf("\nTEST 1: Push operation\n");

    if (push(&s, 10)) {
        printf("PASS: Element pushed successfully\n");
        passed++;
    } else {
        printf("FAIL: Push operation failed\n");
    }

    // TEST 2: Pop operation
    printf("\nTEST 2: Pop operation\n");

    if (pop(&s, &value) && value == 10) {
        printf("PASS: Element popped successfully\n");
        passed++;
    } else {
        printf("FAIL: Pop operation failed\n");
    }

    // TEST 3: LIFO order
    printf("\nTEST 3: LIFO order\n");

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    if (pop(&s, &value) && value == 30) {
        printf("PASS: LIFO order maintained\n");
        passed++;
    } else {
        printf("FAIL: LIFO order incorrect\n");
    }

    // TEST 4: Stack overflow
    printf("\nTEST 4: Stack overflow\n");

    initStack(&s);

    int success = 1;

    for (int i = 0; i < STACK_SIZE; i++) {
        if (!push(&s, i)) {
            success = 0;
            break;
        }
    }

    if (success && !push(&s, 999)) {
        printf("PASS: Stack overflow correctly detected\n");
        passed++;
    } else {
        printf("FAIL: Stack overflow test failed\n");
    }

    // TEST 5: Stack underflow
    printf("\nTEST 5: Stack underflow\n");

    initStack(&s);

    if (!pop(&s, &value)) {
        printf("PASS: Stack underflow correctly detected\n");
        passed++;
    } else {
        printf("FAIL: Stack underflow test failed\n");
    }

    printf("\n=================================\n");
    printf("STACK TESTS PASSED: %d/%d\n", passed, total);
    printf("=================================\n");

    return 0;
}
