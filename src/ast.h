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
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  int line;
  union {
    struct {
      const char *value;
    } literal;                      // Constants

    struct {
      struct ASTNode *left;
      struct ASTNode *right;
      Token op;
    } binaryExpr;                   // +, -, /, * ...

    struct {
      const char *varName;
      struct ASTNode *val;
    } varDecl;

    struct {
      const char *varName;
    } varRef;

    struct {
      struct ASTNode *ifBlock;
      struct ASTNode *elseifBlock;  // Optional
      struct ASTNode *elseBlock;    // Optional
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
      struct ASTNode **statements;
      int count;
      int capacity;
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
  fprintf(stderr, "Expected %d got %d\n", type, currentToken(p)->type);
  exit(EXIT_FAILURE);
}

ASTNode *parseExpression(Parser *p) {
  // I'll do this later
  return NULL;
}

ASTNode *primaryParser(Parser *p) {
  // Parses the main body of the code
  // Does not include expressions!!
  MToken *token = currentToken(p);
  if (token->type == Iden) {
    // Variable reference
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VAR_REF;
    node->as.varRef.varName = token->value;
    advance(p);
    return node;
  }
  if (token->type == Num || token->type == String || token->type == Bool || token->type == Null || token->type == Float) {
    // Constant values
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_LITERAL;
    node->as.literal.value = token->value;
    advance(p);
    return node;
  }
  if (token->type == Opclb) {
    // Blocks of code (for loops, if statements, etc.)
    expect(p, Opclb);
    ASTNode *block = malloc(sizeof(ASTNode));
    block->type = AST_BLOCK;
    block->as.block.statements = malloc(sizeof(ASTNode *)*10);
    block->as.block.count = 0;
    block->as.block.capacity = 10;

    while (currentToken(p)->type != Clclb) {
      if (block->as.block.count == block->as.block.capacity) {
        block->as.block.capacity *= 2;
        block->as.block.statements = realloc(block->as.block.statements, block->as.block.capacity * sizeof(ASTNode *));
      }
      block->as.block.statements[block->as.block.count++] = parseExpression(p);
    }
    expect(p, Clclb);
    return block;
  }
  if (token->type == If) {
    // If statements
    expect(p, If);
    expect(p, Opb);
    ASTNode *condition = parseExpression(p);
    expect(p, Clb);
    expect(p, Do);
  }
}