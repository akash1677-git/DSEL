
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "suffix_tree.h"

#define REPEATS 1000

int naiveSearch(const char *text, const char *pattern) {
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0 || m > n) return 0;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;

        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }

        if (j == m) return 1;
    }

    return 0;
}

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
    printf("       DSEL SEARCH COMPARISON\n");
    printf("========================================\n");

    for (int s = 0; s < sizeCount; s++) {
        char prompt[MAX_TEXT];
        int targetSize = sizes[s];
        int baseLength = (int)strlen(base);
        int pos = 0;

        while (pos + baseLength < targetSize - 1) {
            memcpy(prompt + pos, base, baseLength);
            pos += baseLength;
        }

        prompt[pos] = '\0';

        // Naive search benchmark
        clock_t start = clock();

        int naiveFound = 0;

        for (int r = 0; r < REPEATS; r++) {
            for (int p = 0; p < patternCount; p++) {
                naiveFound += naiveSearch(prompt, patterns[p]);
            }
        }

        clock_t end = clock();

        double naiveTime =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        // Suffix tree construction + search benchmark
        start = clock();

        int suffixFound = 0;

        for (int r = 0; r < REPEATS; r++) {
            SuffixTree tree;
            initSuffixTree(&tree, prompt);

            for (int p = 0; p < patternCount; p++) {
                suffixFound += searchSuffixTree(&tree, patterns[p]);
            }

            freeSuffixTree(&tree);
        }

        end = clock();

        double suffixTime =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        printf("\nPrompt length: %d characters\n", pos);
        printf("Naive search total: %.4f ms\n", naiveTime);
        printf("Suffix tree total: %.4f ms\n", suffixTime);
        printf("Naive matches: %d\n", naiveFound);
        printf("Suffix tree matches: %d\n", suffixFound);
    }

    printf("\n========================================\n");
    printf("       COMPARISON COMPLETE\n");
    printf("========================================\n");

    return 0;
}
