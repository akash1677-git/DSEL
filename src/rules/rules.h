
#ifndef RULES_H
#define RULES_H

#include "hash_table.h"

#define INJECTION_RULE_COUNT 48

typedef struct {
    const char *pattern;
    const char *category;
} SecurityRule;

extern const SecurityRule injectionRules[INJECTION_RULE_COUNT];

void loadSecurityRules(HashTable *ht);

#endif
