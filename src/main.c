
#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "queue.h"
#include "detector.h"
#include "suffix_tree.h"
#include "rules.h"

int main(void) {

    HashTable ht;
    Queue q;

    initHashTable(&ht);
    initQueue(&q);

    // Load centralized security rules
    loadSecurityRules(&ht);

    char prompt[PROMPT_SIZE];
    char processed[PROMPT_SIZE];
    char choice[20];

    printf("=================================\n");
    printf("       DSEL SECURITY ENGINE\n");
    printf("=================================\n");

    printf("\nLoaded %d injection signatures.\n",
           INJECTION_RULE_COUNT);

    while (1) {

        printf("\nEnter prompt: ");

        if (fgets(prompt, sizeof(prompt), stdin) == NULL) {
            printf("\nNo input received. Exiting.\n");
            break;
        }

        // Check for oversized input
        if (strchr(prompt, '\n') == NULL) {

            int ch = getchar();

            if (ch != EOF) {

                while (ch != '\n' && ch != EOF) {
                    ch = getchar();
                }

                printf("\nError: Prompt exceeds the input buffer.\n");
                continue;
            }
        }

        // Remove newline and carriage return
        prompt[strcspn(prompt, "\r\n")] = '\0';

        // Reject empty input
        if (prompt[0] == '\0') {
            printf("\nError: Empty prompt is not allowed.\n");
            continue;
        }

        // Enforce suffix tree input limit
        if (strlen(prompt) > MAX_TEXT - 2) {
            printf("\nError: Prompt exceeds maximum allowed length.\n");
            continue;
        }

        // Enqueue prompt
        if (!enqueue(&q, prompt)) {
            printf("\nQueue is full!\n");
            break;
        }

        // Dequeue prompt
        if (!dequeue(&q, processed)) {
            printf("\nError: Queue processing failed.\n");
            break;
        }

        // Inspect prompt
        Decision result = inspectPrompt(&ht, processed);

        printf("\nPrompt: %s\n", processed);

        switch (result) {

            case ALLOW:
                printf("\nDECISION: ALLOW\n");
                break;

            case FLAG:
                printf("\nDECISION: FLAG\n");
                break;

            case BLOCK:
                printf("\nDECISION: BLOCK\n");
                break;
        }

        // Ask whether the user wants to continue
        while (1) {

            printf("\nWould you like to enter another prompt? [y/n]: ");

            if (fgets(choice, sizeof(choice), stdin) == NULL) {
                printf("\nExiting DSEL.\n");
                goto cleanup;
            }

            choice[strcspn(choice, "\r\n")] = '\0';

            if ((choice[0] == 'y' || choice[0] == 'Y') &&
                choice[1] == '\0') {
                break;
            }

            if ((choice[0] == 'n' || choice[0] == 'N') &&
                choice[1] == '\0') {
                printf("\nThank you for using DSEL!\n");
                goto cleanup;
            }

            printf("Invalid choice. Please enter y or n.\n");
        }
    }

cleanup:

    freeHashTable(&ht);

    return 0;
}
