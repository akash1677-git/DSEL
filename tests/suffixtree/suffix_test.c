
#include <stdio.h>
#include "suffix_tree.h"

int main(void) {

    SuffixTree tree;

    initSuffixTree(&tree, "BANANA");

    if (tree.root == NULL) {
        printf("Suffix tree construction failed.\n");
        return 1;
    }

    printf("=================================\n");
    printf("       DSEL SUFFIX TREE TEST\n");
    printf("=================================\n");

    struct {
        const char *pattern;
        int expected;
    } tests[] = {
        {"ANA", 1},
        {"BAN", 1},
        {"NAN", 1},
        {"ANAx", 0},
        {"APPLE", 0},
        {"A", 1},
        {"BANANA", 1},
        {"NANA", 1},
        {"", 0},
        {"$", 0}
    };

    int total = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;

    for (int i = 0; i < total; i++) {

        int result = searchSuffixTree(&tree, tests[i].pattern);

        printf("\nTEST %d: Pattern \"%s\"\n", i + 1,
               tests[i].pattern);

        if (result == tests[i].expected) {
            printf("RESULT: PASS\n");
            passed++;
        } else {
            printf("RESULT: FAIL\n");
            printf("Expected: %d | Actual: %d\n",
                   tests[i].expected, result);
        }
    }

    freeSuffixTree(&tree);

    printf("\n=================================\n");
    printf("SUFFIX TREE TESTS: %d/%d\n", passed, total);
    printf("=================================\n");

    return (passed == total) ? 0 : 1;
}
