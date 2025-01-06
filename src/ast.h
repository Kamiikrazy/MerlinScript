#include "defs.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  TokenList *tokens;
  int current;
} Parser;

Parser newParser(TokenList *tokens) {
  return (Parser){.tokens = tokens, .current = 0};
}

MToken *currentToken(Parser *p) {
  return &p->tokens->tokens[p->current];
}

void advance(Parser *p) {
  p->current++;
}

void error(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
}

int match(Parser *p, Token type) {
  if (currentToken(p)->type == type) {
    advance(p);
    return 1;
  }
  return 0;
}

int expect(Parser *p, Token type) {
  if (match(p, type)) return 1;
  fprintf(stderr, "Expected %d, got %d\n", type, currentToken(p)->type);
  exit(EXIT_FAILURE);
}