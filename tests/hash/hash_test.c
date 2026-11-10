
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

int main() {
    HashTable ht;
    initHashTable(&ht);

    int passed = 0;
    int total = 5;

    printf("=================================\n");
    printf("       DSEL HASH TABLE TESTS\n");
    printf("=================================\n");

    // TEST 1: Insert a rule
    printf("\nTEST 1: Rule insertion\n");

    insertRule(&ht, "ignore previous instructions", "INJECTION");

    if (searchRule(&ht, "ignore previous instructions")) {
        printf("PASS: Rule inserted successfully\n");
        passed++;
    } else {
        printf("FAIL: Rule insertion failed\n");
    }

    // TEST 2: Search existing rule
    printf("\nTEST 2: Search existing rule\n");

    if (searchRule(&ht, "ignore previous instructions")) {
        printf("PASS: Existing rule found\n");
        passed++;
    } else {
        printf("FAIL: Existing rule not found\n");
    }

    // TEST 3: Search nonexistent rule
    printf("\nTEST 3: Search nonexistent rule\n");

    if (!searchRule(&ht, "normal educational prompt")) {
        printf("PASS: Nonexistent rule correctly not found\n");
        passed++;
    } else {
        printf("FAIL: Unexpected rule found\n");
    }

    // TEST 4: Collision handling
    printf("\nTEST 4: Collision handling\n");

    // Find two different strings that map to the same bucket
    char first[50];
    char second[50];
    int collisionFound = 0;

    for (int i = 0; i < 1000 && !collisionFound; i++) {
        snprintf(first, sizeof(first), "rule_%d", i);

        for (int j = i + 1; j < 1000; j++) {
            snprintf(second, sizeof(second), "rule_%d", j);

            unsigned int h1 = 0;
            unsigned int h2 = 0;

            for (int k = 0; first[k]; k++)
                h1 = (h1 * 31 + (unsigned char)first[k]) % TABLE_SIZE;

            for (int k = 0; second[k]; k++)
                h2 = (h2 * 31 + (unsigned char)second[k]) % TABLE_SIZE;

            if (h1 == h2) {
                collisionFound = 1;
                break;
            }
        }
    }

    if (collisionFound) {
        insertRule(&ht, first, "TEST");
        insertRule(&ht, second, "TEST");

        if (searchRule(&ht, first) &&
            searchRule(&ht, second)) {
            printf("PASS: Both colliding rules found\n");
            passed++;
        } else {
            printf("FAIL: Collision handling failed\n");
        }
    } else {
        printf("FAIL: Could not generate collision\n");
    }

    // TEST 5: Memory cleanup
    printf("\nTEST 5: Memory cleanup\n");

    freeHashTable(&ht);

    printf("PASS: Hash table cleanup function executed\n");
    passed++;

    printf("\n=================================\n");
    printf("HASH TESTS PASSED: %d/%d\n", passed, total);
    printf("=================================\n");

    return 0;
}
