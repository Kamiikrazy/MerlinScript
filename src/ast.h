#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

// Define the different nodes
typedef enum {
  AST_LITERAL,
  AST_BINARY_EXPR,
  AST_UNARY_EXPR,
  AST_VAR_DECL,
  AST_FUNC_DECL,
  AST_BLOCK,
  AST_IF,
  AST_WHILE,
  AST_FOR,
  AST_TRY,
  AST_VAR_REF,
  AST_MODULE,
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  int line;
  union {
    struct {
      const char *value;
    } literal;

    struct {
      struct ASTNode *left;
      struct ASTNode *right;
      Token op;
    } binaryExpr;                // +, -, /, * ...

    struct {
      const char *varName;
      struct ASTNode *val;
    } varDecl;

    struct {
      const char *varName;
    } varRef;

    struct {
      struct ASTNode *ifBlock;
      struct ASTNode *thenBlock;
      struct ASTNode *elseBlock; // Optional
      struct ASTNode *doBlock;   // Optional
    } ifBlock;

    struct {
      struct ASTNode *whileBlock;
      struct ASTNode *doBlock;
      struct ASTNode *body;
    } whileBlock;

    struct {
      struct ASTNode *forBlock;
      struct ASTNode *doBlock;
      struct ASTNode *body;
    } forBlock;

    struct {
      struct ASTNode *tryBlock;
      struct ASTNode *tryBody;
      struct ASTNode *catchBlock;
      struct ASTNode *catchBody;
    } tryBlock;

    struct {
      struct ASTNode *module;
      struct ASTNode *package;
    } moduleBlock;

    struct {
      struct ASTNode **statements;
      int count;
    } block;
  } as;
} ASTNode;

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