
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "detector.h"
#include "stack.h"
#include "suffix_tree.h"
#include "queue.h"
#include "rules.h"

// Case-insensitive search returning the matching position
static const char *findIgnoreCase(const char *text, const char *pattern) {

    size_t m = strlen(pattern);

    if (m == 0) return text;

    for (size_t i = 0; text[i] != '\0'; i++) {

        size_t j = 0;

        while (j < m && text[i + j] != '\0' &&
               tolower((unsigned char)text[i + j]) ==
               tolower((unsigned char)pattern[j])) {
            j++;
        }

        if (j == m) return text + i;
    }

    return NULL;
}

// Detect API keys and credential-like strings
static int detectSecret(const char *prompt) {

    // Detect API keys: sk- followed by at least 10 alphanumeric characters
    const char *position = prompt;

    while ((position = findIgnoreCase(position, "sk-")) != NULL) {

        const char *key = position + 3;
        int count = 0;

        while (isalnum((unsigned char)*key)) {
            count++;
            key++;
        }

        if (count >= 10) return 1;

        position += 3;
    }

    // Common credential labels
    const char *labels[] = {
        "password",
        "passwd",
        "api_key",
        "secret",
        "access_token"
    };

    int labelCount = 5;

    for (int i = 0; i < labelCount; i++) {

        position = prompt;

        // Check every occurrence of each credential label
        while ((position = findIgnoreCase(position, labels[i])) != NULL) {

            const char *found = position + strlen(labels[i]);

            while (*found == ' ' || *found == '\t') {
                found++;
            }

            // Check separators
            if (*found == '=' || *found == ':') {
                found++;
            }
            else if (tolower((unsigned char)found[0]) == 'i' &&
                     found[1] != '\0' &&
                     tolower((unsigned char)found[1]) == 's' &&
                     found[2] == ' ') {
                found += 3;
            }
            else {
                position++;
                continue;
            }

            while (*found == ' ' || *found == '\t') {
                found++;
            }

            int valueLength = 0;

            while (*found != '\0' &&
                   *found != ' ' &&
                   *found != '\t' &&
                   *found != '\n') {
                valueLength++;
                found++;
            }

            if (valueLength >= 4) return 1;

            position++;
        }
    }

    return 0;
}

// Check whether an injection category has already been counted
static int categoryAlreadyCounted(
    char categories[][50],
    int count,
    const char *category
) {

    for (int i = 0; i < count; i++) {

        if (strcmp(categories[i], category) == 0) {
            return 1;
        }
    }

    return 0;
}

// Main prompt inspection function
Decision inspectPrompt(HashTable *ht, const char *prompt) {

    // Reject invalid input
    if (ht == NULL || prompt == NULL || prompt[0] == '\0') {

        printf("Inspection error: Invalid input.\n");
        return BLOCK;
    }

    size_t promptLength = strlen(prompt);

    if (promptLength > MAX_TEXT - 2 ||
        promptLength >= PROMPT_SIZE) {

        printf("Inspection error: Prompt exceeds maximum allowed length.\n");
        return BLOCK;
    }

    int risk = 0;

    Stack s;
    initStack(&s);

    // Convert prompt to lowercase
    char lowerPrompt[PROMPT_SIZE];

    memcpy(lowerPrompt, prompt, promptLength + 1);

    for (size_t i = 0; lowerPrompt[i] != '\0'; i++) {
        lowerPrompt[i] =
            (char)tolower((unsigned char)lowerPrompt[i]);
    }

    // Build suffix tree
    SuffixTree st;
    initSuffixTree(&st, lowerPrompt);

    // Fail closed if suffix tree construction fails
    if (st.root == NULL) {

        printf("Inspection error: Suffix tree construction failed.\n");
        freeSuffixTree(&st);

        return BLOCK;
    }

    // Store distinct matched categories
    char matchedCategories[INJECTION_RULE_COUNT][50];
    int categoryCount = 0;

    printf("\n--- INJECTION SCAN ---\n");

    // Search all centralized injection signatures
    for (int i = 0; i < INJECTION_RULE_COUNT; i++) {

        const char *pattern = injectionRules[i].pattern;
        const char *category = injectionRules[i].category;

        if (searchSuffixTree(&st, pattern)) {

            printf("Suffix Tree Match: %s\n", pattern);

            // Report the corresponding hash table rule
            if (searchRule(ht, pattern)) {

                printf("Threat detected: %s\n", category);
                printf("Matched rule: %s\n", pattern);
            }

            // Count each distinct category only once
            if (!categoryAlreadyCounted(
                    matchedCategories,
                    categoryCount,
                    category)) {

                if (categoryCount >= INJECTION_RULE_COUNT ||
                    !push(&s, 1)) {

                    printf("Inspection error: Risk stack overflow.\n");
                    freeSuffixTree(&st);

                    return BLOCK;
                }

                snprintf(
                    matchedCategories[categoryCount],
                    sizeof(matchedCategories[categoryCount]),
                    "%s",
                    category
                );

                categoryCount++;
            }
        }
    }

    // Calculate injection risk
    int value;

    while (pop(&s, &value)) {

        if (value) {
            risk += 40;
        }
    }

    printf("\nDistinct injection categories detected: %d\n",
           categoryCount);

    // Detect exposed credentials
    if (detectSecret(prompt)) {

        printf("Sensitive credential pattern detected!\n");

        risk += 60;
    }

    printf("\nRisk Score: %d\n", risk);

    // Release suffix tree memory
    freeSuffixTree(&st);

    // Final security decision
    if (risk >= 60) return BLOCK;
    if (risk > 0) return FLAG;

    return ALLOW;
}
