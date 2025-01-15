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
      struct ASTNode **elifs;       // Optional
      int elifCount;
      struct ASTNode *elseBlock;    // Optional
    } ifStatement;

    struct {
      struct ASTNode *condition;
      struct ASTNode *whileBlock;
    } whileStatement;

    struct {
      struct ASTNode *iterator;
      struct ASTNode *start;
      struct ASTNode *end;
      struct ASTNode *body;
    } forStatement;

    struct {
      struct ASTNode *tryBlock;
      struct ASTNode *catchBlock;
    } tryStatement;

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

void throwError(const char *msg) {
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

ASTNode *parseCondition(Parser *p) {

}

ASTNode *parseBlock(Parser *p) {
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
    block->as.block.statements[block->as.block.count++] = parseCondition(p);
  }
  return block;
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
      block->as.block.statements[block->as.block.count++] = parseCondition(p);
    }
    expect(p, Clclb);
    return block;
  }
  if (token->type == If) {
    // If statements
    /*
    if (condition) do {
      ...
    } otherwise if (condition) do {
      ...
    } otherwise do {
      ...
    }
    */
    ASTNode *ifNode = malloc(sizeof(ASTNode));
    ifNode->type = AST_IF;

    expect(p, If);
    expect(p, Opb);
    ASTNode *condition = parseCondition(p);
    expect(p, Clb);
    expect(p, Do);

    ifNode->as.ifStatement.ifBlock = parseBlock(p);

    ifNode->as.ifStatement.elifs = NULL;
    ifNode->as.ifStatement.elifCount = 0;
    ifNode->as.ifStatement.elseBlock = NULL;

    // Parse the orif statements
    while (match(p, Orif)) {
      ifNode->as.ifStatement.elifCount++;
      ifNode->as.ifStatement.elifs = realloc(ifNode->as.ifStatement.elifs, ifNode->as.ifStatement.elifCount * sizeof(ASTNode *));
      expect(p, Orif);
      expect(p, Opb);
      ifNode->as.ifStatement.elifs[ifNode->as.ifStatement.elifCount - 1] = parseCondition(p);
      expect(p, Clb);
      expect(p, Do);
      ifNode -> as.ifStatement.elifs[ifNode->as.ifStatement.elifCount - 1] = parseBlock(p);
    }

    // Else
    if (match(p, Else)) {
      ifNode->as.ifStatement.elseBlock = parseBlock(p);
    }
    return ifNode;
  }
  if (token->type == While) {
    // While statements
    /*
    while (condition) do {
      ...
    }
    */
    ASTNode *whileNode = malloc(sizeof(ASTNode));
    whileNode->type = AST_WHILE;
    expect(p, While);
    expect(p, Opb);
    ASTNode *condition = parseCondition(p);
    if (!condition) {
      throwError("Expected condition");
    }
    expect(p, Clb);
    expect(p, Do);
    ASTNode *block = parseBlock(p);
    whileNode->as.whileStatement.condition = condition;
    whileNode->as.whileStatement.whileBlock = block;
    return whileNode;
  }

  if (token->type == For) {
    // For statements
    /*
    for(i in 1, 3) do {
      ...
    }
    */
    ASTNode *forNode = malloc(sizeof(ASTNode));
    forNode->type = AST_FOR;
    expect(p, For);
    expect(p, Opb);
    if (currentToken(p)->type == Iden) {
      throwError("Expected variable name");
    }
    // Variable for iteration
    ASTNode *variable = malloc(sizeof(ASTNode));
    variable->type = AST_VAR_REF;
    variable->as.varRef.varName = currentToken(p)->value;
    advance(p); // Skip
    expect(p, In);
    // Starting range
    if (currentToken(p)->type != Num) {
      throwError("Invalid range");
    }
    ASTNode *start = malloc(sizeof(ASTNode));
    start->type = AST_LITERAL;
    start->as.literal.value = currentToken(p)->value;
    advance(p); // Skip
    expect(p, Comma);
    // Ending range
    if (currentToken(p)->type != Num) {
       throwError("Invalid range");
    }
    ASTNode *end = malloc(sizeof(ASTNode));
    end->type = AST_LITERAL;
    end->as.literal.value = currentToken(p)->value;
    advance(p); // Skip
    expect(p, Clb);
    expect(p, Do);
    // Body
    ASTNode *body = parseBlock(p);
    forNode->as.forStatement.iterator = variable;
    forNode->as.forStatement.start = start;
    forNode->as.forStatement.end = end;
    forNode->as.forStatement.body = body;

    return forNode;
  }
}