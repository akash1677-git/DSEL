
#include <stdio.h>
#include "suffix_tree.h"

int main(void) {

    const char *text = "BANANA BANDANA";

    int repetitions = 10000;

    printf("DSEL SUFFIX TREE STRESS TEST\n");

    for (int i = 0; i < repetitions; i++) {

        SuffixTree tree;

        initSuffixTree(&tree, text);

        if (tree.root == NULL) {
            printf("FAIL: Construction failed at iteration %d\n", i);
            return 1;
        }

        if (!searchSuffixTree(&tree, "BAN") ||
            !searchSuffixTree(&tree, "NANA") ||
            searchSuffixTree(&tree, "APPLE")) {

            printf("FAIL: Search error at iteration %d\n", i);
            freeSuffixTree(&tree);
            return 1;
        }

        freeSuffixTree(&tree);
    }

    printf("PASS: %d construction/search/free cycles completed.\n",
           repetitions);

    return 0;
}
