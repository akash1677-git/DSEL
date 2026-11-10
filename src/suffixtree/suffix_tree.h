
#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#define MAX_TEXT 1000
#define MAX_NODES 2000

typedef struct SuffixNode {
    int start;
    int end;
    struct SuffixNode *children[256];
    struct SuffixNode *suffixLink;
} SuffixNode;

typedef struct {
    char text[MAX_TEXT];
    SuffixNode *root;
} SuffixTree;

void initSuffixTree(SuffixTree *tree, const char *text);
int searchSuffixTree(SuffixTree *tree, const char *pattern);
void freeSuffixTree(SuffixTree *tree);

#endif
