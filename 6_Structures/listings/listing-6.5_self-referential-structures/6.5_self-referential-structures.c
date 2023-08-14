#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

struct tnode {          /* the tree node: */
    char *word;             /* points to the next */
    int count;              /* number of occurrences */
    struct tnode *left;     /* left child */
    struct tnode *right;    /* right child */
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

struct tnode *talloc(void);
char *str_dup(char *s);

/* word frequency count */
int main() {
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0]))
            root = addtree(root, word);
    }
    treeprint(root);
    return 0;
}

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;

    if (p == NULL) {        /* a new word has arrived */
        p = talloc();       /* make a new node */
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;         /* repeated word */
    } else if (cond < 0) {  /* less than into left subtree */
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}

/* talloc: make a tnode */
struct tnode *talloc(void) {
    return (struct tnode *) malloc(sizeof(struct tnode));
}

/* strdup: make a duplicate of s*/
char *str_dup(char *s) {
    char *p;

    p = (char *) malloc(strlen(s) + 1);     /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

/* getword: get next word or character from input */
int getword(char *word, int lim) {
    int c;
    char *w = word;
    int t;

    while (isspace(c = getchar()))
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) {
        if (c == '\"') { /* string constant */
            for (c = getchar(); c != '\"'; c = getchar())
                ;
        } else if (c == '#') { /* preprocessor */ 
            for (c = getchar(); c != '\n'; c = getchar())
                ;
        } else if (c == '/') { /* comment */
            if ((c = getchar()) == '/') { /* single comment */
                for (c = getchar(); c != '\n'; c = getchar())
                    ;
            }
            else if (c == '*') { /* multiline comment */
                for (c = getchar(), t = getchar(); c != '*' && t != '/'; c = getchar(), t = getchar())
                    ungetc(t, stdin);
            } else {
                ungetc(c, stdin);
            }
        } else { /* underscore */
            for (; !isspace(c) && c != EOF; c = getchar());
        }
        if (c != '\"' && c != '\n' && c != '/')
            ungetc(c, stdin);
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getchar())) {
            ungetc(*w, stdin);
            break;
        }
    *w = '\0';
    return word[0];
}