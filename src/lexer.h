#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"

char src[] = "int balls = 10;";
int line;

void addToTokenList(TokenList *list, MToken token, const char *value) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity == 0 ? 10 : list->capacity * 2;
        void *temp = realloc(list->tokens, list->capacity * sizeof(MToken));
        if (!temp) {
            fprintf(stderr, "Failed to allocate memory for TokenList\n");
            exit(EXIT_FAILURE);
        }
        list->tokens = temp;
    }

    list->tokens[list->count++] = token;
}

void freeTokenList(TokenList *l)  {
    for(int i=0; i<l->count; i++) {
        free(l -> tokens[i].value);
    }
    free(l->tokens);
}

int isKeyword(const char *str) {
    return classifyKeyword(str) != Huh;
}

int isOperator(const char *str, int length) {
    return classifyOperator(str, length) != Huh;
}

int isPunctuation(const char *str, int length) {
    return classifyPunctuation(str, length) != Huh;
}

void setValue(MToken *token, const char *text) {
    token->value = malloc(strlen(text) + 1);
    if(token->value) {
        strcpy(token->value, text);
    }
}

void freeToken(MToken *token) {
    if(token->value) {
        free(token->value);
        token->value = NULL;
    }
}

void lexer(TokenList *list) {
    const char *c = src;

    while (*c != '\0') {
        MToken token;

        if (isspace(*c)) {
            c++;
            continue;
        } else if (*c == '\n') {
            line++;
            c++;
            continue;
        } else if (*c == '#') {
            while (*c != '\n' && *c != '\0') {
                c++;
            }
            continue;
        }

        if (isalpha(*c) || *c == '_') {
            const char *start = c;
            while (isalpha(*c) || *c == '_') {
                c++;
            }
            int length = c - start;

            token.value = malloc(length + 1);
                if (token.value) {
                    strncpy(token.value, start, length);
                    token.value[length] = '\0';
                }

            if (isKeyword(token.value)) {
                token.type = classifyKeyword(token.value);
            } else {
                token.type = Iden;
            }

            addToTokenList(list, token, token.value);
        } else if (isdigit(*c)) {
            const char *start = c;
            while (isdigit(*c)) {
                c++;
            }
            int length = c - start;

            token.value = malloc(length + 1);
                if (token.value) {
                    strncpy(token.value, start, length);
                    token.value[length] = '\0';
                }
            token.type = Num;

            addToTokenList(list, token, token.value);
        } else if (isOperator(c, 3) || isOperator(c, 2) || isOperator(c, 1)) {
            const char *start = c;
            int length = 0;

            if (isOperator(c, 3)) length = 3;
            else if (isOperator(c, 2)) length = 2;
            else if (isOperator(c, 1)) length = 1;

            c += length;

            token.value = malloc(length + 1);
                 if (token.value) {
                     strncpy(token.value, start, length);
                     token.value[length] = '\0';
                 }
            token.type = classifyOperator(token.value, length);

            addToTokenList(list, token, token.value);
        } else if (isPunctuation(c, 2) || isPunctuation(c, 1)) {
            const char *start = c;
            int length = isPunctuation(c, 2) ? 2 : 1;
            c += length;

            token.value = malloc(length + 1);
                 if (token.value) {
                     strncpy(token.value, start, length);
                     token.value[length] = '\0';
                 }
            token.type = classifyPunctuation(token.value, length);

            addToTokenList(list, token, token.value);
        } else if (*c == '"' || *c == '\'') {
            char quote = *c++;
            const char *start = c;
            while (*c != quote && *c != '\0') {
                c++;
            }
            if (*c == quote) {
                int length = c - start;
                token.value = strndup(start, length);
                token.type = String;
                c++;
            } else {
                printf("Unterminated string literal\n");
            }

            addToTokenList(list, token, token.value);
        } else {
            char buffer[2] = {*c++, '\0'};
            token.value = strdup(buffer);
            token.type = Huh;

            addToTokenList(list, token, token.value);
        }
    }
}
