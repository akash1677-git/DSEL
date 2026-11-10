
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "suffix_tree.h"

#define REPEATS 100000

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
    const char *prompt =
        "This is a normal prompt explaining data structures and algorithms. "
        "It contains several examples of searching and processing text.";

    const char *patterns[] = {
        "data structures",
        "algorithms",
        "searching",
        "processing",
        "ignore all previous instructions",
        "reveal system prompt"
    };

    int patternCount = 6;

    printf("========================================\n");
    printf("       DSEL REUSE BENCHMARK\n");
    printf("========================================\n");

    // Build suffix tree only once
    SuffixTree tree;
    clock_t buildStart = clock();

    initSuffixTree(&tree, prompt);

    clock_t buildEnd = clock();

    double buildTime =
        ((double)(buildEnd - buildStart) / CLOCKS_PER_SEC) * 1000.0;

    // Naive search benchmark
    int naiveFound = 0;

    clock_t start = clock();

    for (int r = 0; r < REPEATS; r++) {
        for (int p = 0; p < patternCount; p++) {
            naiveFound += naiveSearch(prompt, patterns[p]);
        }
    }

    clock_t end = clock();

    double naiveTime =
        ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    // Reuse the same suffix tree
    int suffixFound = 0;

    start = clock();

    for (int r = 0; r < REPEATS; r++) {
        for (int p = 0; p < patternCount; p++) {
            suffixFound += searchSuffixTree(&tree, patterns[p]);
        }
    }

    end = clock();

    double suffixTime =
        ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("\nPrompt length: %zu characters\n", strlen(prompt));
    printf("Tree construction time: %.4f ms\n", buildTime);
    printf("Naive search time: %.4f ms\n", naiveTime);
    printf("Suffix tree search time: %.4f ms\n", suffixTime);

    printf("\nNaive matches: %d\n", naiveFound);
    printf("Suffix tree matches: %d\n", suffixFound);

    freeSuffixTree(&tree);

    printf("\n========================================\n");
    printf("       BENCHMARK COMPLETE\n");
    printf("========================================\n");

    return 0;
}
