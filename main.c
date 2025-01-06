#include <stdio.h>
#include <stdlib.h>
#include "src/lexer.h"

int main() {
    TokenList list = {NULL, 0, 0};
    line = 1;

    lexer(&list);

    for (int i = 0; i < list.count; i++) {
        MToken token = list.tokens[i];
        printf("Token: %d, Value: '%s'\n", token.type, token.value);
    }

    freeTokenList(&list);
    return 0;
}