#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "suffix_tree.h"

#define REPEATS 1000

int main() {
    const char *base =
        "This is a normal prompt explaining data structures and algorithms. ";

    const char *patterns[] = {
        "data structures",
        "algorithms",
        "ignore all previous instructions",
        "reveal system prompt"
    };

    int patternCount = 4;

    int sizes[] = {100, 250, 500, 750, 900};
    int sizeCount = 5;

    printf("========================================\n");
    printf("       DSEL SUFFIX TREE BENCHMARK\n");
    printf("========================================\n");

    for (int s = 0; s < sizeCount; s++) {
        int targetSize = sizes[s];

        char prompt[MAX_TEXT];
        int baseLength = (int)strlen(base);
        int pos = 0;

        while (pos + baseLength < targetSize - 1) {
            memcpy(prompt + pos, base, baseLength);
            pos += baseLength;
        }

        prompt[pos] = '\0';

        // Construction benchmark
        clock_t start = clock();

        for (int r = 0; r < REPEATS; r++) {
            SuffixTree tree;
            initSuffixTree(&tree, prompt);
            freeSuffixTree(&tree);
        }

        clock_t end = clock();

        double constructionTime =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0 / REPEATS;

        // Build one tree for search benchmark
        SuffixTree tree;
        initSuffixTree(&tree, prompt);

        start = clock();

        int foundCount = 0;

        for (int r = 0; r < REPEATS; r++) {
            for (int p = 0; p < patternCount; p++) {
                foundCount += searchSuffixTree(&tree, patterns[p]);
            }
        }

        end = clock();

        double searchTime =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0 / REPEATS;

        printf("\nPrompt size: %d characters\n", pos);
        printf("Construction time: %.4f ms\n", constructionTime);
        printf("Search time (4 patterns): %.4f ms\n", searchTime);
        printf("Total successful searches: %d\n", foundCount);

        freeSuffixTree(&tree);
    }

    printf("\n========================================\n");
    printf("          BENCHMARK COMPLETE\n");
    printf("========================================\n");

    return 0;
}
