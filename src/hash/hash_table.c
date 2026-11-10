
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// Hash function using polynomial hashing
static int hashFunction(const char *pattern) {

    unsigned long hash = 0;

    for (size_t i = 0; pattern[i] != '\0'; i++) {
        hash = (hash * 31 + (unsigned char)pattern[i]) % TABLE_SIZE;
    }

    return (int)hash;
}

// Initialize hash table
void initHashTable(HashTable *ht) {

    if (ht == NULL)
        return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

// Insert a security rule
void insertRule(HashTable *ht,
                const char *pattern,
                const char *category) {

    if (ht == NULL || pattern == NULL ||
        category == NULL || pattern[0] == '\0' ||
        category[0] == '\0') {
        return;
    }

    int index = hashFunction(pattern);

    // Prevent duplicate patterns
    Node *temp = ht->table[index];

    while (temp != NULL) {

        if (strcmp(temp->pattern, pattern) == 0) {
            return;
        }

        temp = temp->next;
    }

    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }

    snprintf(newNode->pattern,
             sizeof(newNode->pattern),
             "%s", pattern);

    snprintf(newNode->category,
             sizeof(newNode->category),
             "%s", category);

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

// Search for a rule
int searchRule(HashTable *ht, const char *pattern) {

    if (ht == NULL || pattern == NULL || pattern[0] == '\0')
        return 0;

    int index = hashFunction(pattern);

    Node *temp = ht->table[index];

    while (temp != NULL) {

        if (strcmp(temp->pattern, pattern) == 0) {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

// Display all security rules
void displayRules(HashTable *ht) {

    if (ht == NULL)
        return;

    printf("\n--- SECURITY RULES ---\n");

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node *temp = ht->table[i];

        while (temp != NULL) {

            printf("[%s] %s\n",
                   temp->category,
                   temp->pattern);

            temp = temp->next;
        }
    }
}

// Free all allocated memory
void freeHashTable(HashTable *ht) {

    if (ht == NULL)
        return;

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node *temp = ht->table[i];

        while (temp != NULL) {

            Node *next = temp->next;

            free(temp);

            temp = next;
        }

        ht->table[i] = NULL;
    }
}
