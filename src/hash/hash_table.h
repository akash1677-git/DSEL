#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 100

typedef struct Node {
    char pattern[150];
    char category[50];
    struct Node *next;
} Node;

typedef struct {
    Node *table[TABLE_SIZE];
} HashTable;

void initHashTable(HashTable *ht);
void insertRule(HashTable *ht, const char *pattern, const char *category);
int searchRule(HashTable *ht, const char *pattern);
void displayRules(HashTable *ht);
void freeHashTable(HashTable *ht);

#endif