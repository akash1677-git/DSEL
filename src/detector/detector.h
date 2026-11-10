#ifndef DETECTOR_H
#define DETECTOR_H

#include "hash_table.h"

typedef enum {
    ALLOW,
    FLAG,
    BLOCK
} Decision;

Decision inspectPrompt(HashTable *ht, const char *prompt);

#endif
