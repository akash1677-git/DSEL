
#include <stdio.h>
#include <string.h>

#include "queue.h"

int main() {
    Queue q;
    char output[PROMPT_SIZE];

    initQueue(&q);

    int passed = 0;
    int total = 5;

    printf("=================================\n");
    printf("       DSEL QUEUE TESTS\n");
    printf("=================================\n");

    // TEST 1: Normal enqueue
    printf("\nTEST 1: Enqueue operation\n");

    if (enqueue(&q, "Prompt 1")) {
        printf("PASS: Enqueue successful\n");
        passed++;
    } else {
        printf("FAIL: Enqueue unsuccessful\n");
    }

    // TEST 2: Fill the queue
    printf("\nTEST 2: Queue overflow\n");

    int success = 1;

    while (q.count < QUEUE_SIZE) {
        char prompt[PROMPT_SIZE];

        snprintf(prompt, sizeof(prompt),
                 "Prompt %d", q.count + 1);

        if (!enqueue(&q, prompt)) {
            success = 0;
            break;
        }
    }

    if (success && !enqueue(&q, "Overflow prompt")) {
        printf("PASS: Queue correctly rejects overflow\n");
        passed++;
    } else {
        printf("FAIL: Queue overflow test failed\n");
    }

    // TEST 3: FIFO order
    printf("\nTEST 3: FIFO order\n");

    success = 1;

    for (int i = 1; i <= QUEUE_SIZE; i++) {
        if (!dequeue(&q, output)) {
            success = 0;
            break;
        }

        char expected[PROMPT_SIZE];

        snprintf(expected, sizeof(expected),
                 "Prompt %d", i);

        if (strcmp(output, expected) != 0) {
            success = 0;
            break;
        }
    }

    if (success) {
        printf("PASS: FIFO order maintained\n");
        passed++;
    } else {
        printf("FAIL: FIFO order incorrect\n");
    }

    // TEST 4: Empty queue
    printf("\nTEST 4: Dequeue from empty queue\n");

    if (!dequeue(&q, output)) {
        printf("PASS: Empty queue correctly detected\n");
        passed++;
    } else {
        printf("FAIL: Empty queue test failed\n");
    }

    // TEST 5: Circular wraparound
    printf("\nTEST 5: Circular wraparound\n");

    success = 1;

    for (int i = 1; i <= QUEUE_SIZE; i++) {
        if (!enqueue(&q, "Wrap test")) {
            success = 0;
            break;
        }
    }

    for (int i = 0; i < QUEUE_SIZE / 2; i++) {
        if (!dequeue(&q, output)) {
            success = 0;
            break;
        }
    }

    for (int i = 0; i < QUEUE_SIZE / 2; i++) {
        if (!enqueue(&q, "New prompt")) {
            success = 0;
            break;
        }
    }

    if (success && q.count == QUEUE_SIZE) {
        printf("PASS: Circular wraparound successful\n");
        passed++;
    } else {
        printf("FAIL: Circular wraparound failed\n");
    }

    printf("\n=================================\n");
    printf("QUEUE TESTS PASSED: %d/%d\n", passed, total);
    printf("=================================\n");

    return 0;
}
