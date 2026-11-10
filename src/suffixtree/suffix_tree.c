
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suffix_tree.h"

// Create a new suffix tree node
static SuffixNode* createNode(int start, int end) {
    SuffixNode *node = calloc(1, sizeof(SuffixNode));

    if (node == NULL)
        return NULL;

    node->start = start;
    node->end = end;
    node->suffixLink = NULL;

    return node;
}

// Free a node and all its descendants
static void freeNode(SuffixNode *node) {
    if (node == NULL)
        return;

    for (int i = 0; i < 256; i++) {
        freeNode(node->children[i]);
    }

    free(node);
}

// Insert one suffix
static int insertSuffix(SuffixTree *tree,
                        int suffixStart,
                        int textLength) {

    SuffixNode *current = tree->root;
    int pos = suffixStart;

    while (pos < textLength) {

        unsigned char index =
            (unsigned char)tree->text[pos];

        SuffixNode *child = current->children[index];

        // Create a leaf if no matching child exists
        if (child == NULL) {

            SuffixNode *leaf =
                createNode(pos, textLength - 1);

            if (leaf == NULL)
                return 0;

            current->children[index] = leaf;
            return 1;
        }

        int edgeStart = child->start;
        int edgeEnd = child->end;
        int k = 0;

        // Compare characters along the edge
        while (pos + k < textLength &&
               edgeStart + k <= edgeEnd &&
               tree->text[pos + k] ==
               tree->text[edgeStart + k]) {
            k++;
        }

        // Entire edge matched
        if (edgeStart + k > edgeEnd) {
            pos += k;
            current = child;
            continue;
        }

        // Prepare split node and new leaf
        SuffixNode *splitNode =
            createNode(edgeStart, edgeStart + k - 1);

        if (splitNode == NULL)
            return 0;

        SuffixNode *newLeaf =
            createNode(pos + k, textLength - 1);

        if (newLeaf == NULL) {
            freeNode(splitNode);
            return 0;
        }

        // Connect the existing child and new leaf
        child->start = edgeStart + k;

        unsigned char oldIndex =
            (unsigned char)tree->text[child->start];

        unsigned char newIndex =
            (unsigned char)tree->text[pos + k];

        splitNode->children[oldIndex] = child;
        splitNode->children[newIndex] = newLeaf;

        current->children[index] = splitNode;

        return 1;
    }

    return 1;
}

// Initialize suffix tree
void initSuffixTree(SuffixTree *tree, const char *text) {

    if (tree == NULL)
        return;

    tree->root = NULL;
    tree->text[0] = '\0';

    if (text == NULL) {
        fprintf(stderr, "Suffix tree: NULL input.\n");
        return;
    }

    size_t len = strlen(text);

    if (len > MAX_TEXT - 2 ||
        strchr(text, '$') != NULL) {
        fprintf(stderr,
                "Suffix tree: invalid or oversized input.\n");
        return;
    }

    memcpy(tree->text, text, len);

    // Append unique terminal character
    tree->text[len] = '$';
    tree->text[len + 1] = '\0';

    int textLength = (int)len + 1;

    tree->root = createNode(-1, -1);

    if (tree->root == NULL) {
        fprintf(stderr, "Suffix tree: root allocation failed.\n");
        return;
    }

    // Insert every suffix
    for (int i = 0; i < textLength; i++) {

        if (!insertSuffix(tree, i, textLength)) {

            fprintf(stderr,
                    "Suffix tree: memory allocation failed.\n");

            freeNode(tree->root);
            tree->root = NULL;
            return;
        }
    }
}

// Search for a substring
int searchSuffixTree(SuffixTree *tree,
                     const char *pattern) {

    if (tree == NULL ||
        tree->root == NULL ||
        pattern == NULL ||
        pattern[0] == '\0') {
        return 0;
    }

    if (strchr(pattern, '$') != NULL)
        return 0;

    SuffixNode *current = tree->root;

    int pos = 0;
    int patternLength = (int)strlen(pattern);

    while (pos < patternLength) {

        unsigned char index =
            (unsigned char)pattern[pos];

        SuffixNode *child = current->children[index];

        if (child == NULL)
            return 0;

        int edgeStart = child->start;
        int edgeEnd = child->end;

        while (edgeStart <= edgeEnd &&
               pos < patternLength) {

            if (tree->text[edgeStart] != pattern[pos])
                return 0;

            edgeStart++;
            pos++;
        }

        if (pos == patternLength)
            return 1;

        current = child;
    }

    return 1;
}

// Release all allocated memory
void freeSuffixTree(SuffixTree *tree) {

    if (tree == NULL)
        return;

    freeNode(tree->root);
    tree->root = NULL;
    tree->text[0] = '\0';
}
