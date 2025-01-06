#include <string.h>

typedef enum {
    Num, Iden, Fn, Pub, Loc, If, Else, Orif, Return, Try, While, For, Pass, Break, Do, Var,
    String, Int, Float, Bool, Null, True, False, Module, Or, And, Not, Eq, Ne, Eqeq, Lt, Gt, Le, Ge,
    Add, Sub, Mul, Div, Mod, Inc, Dec, Muleq, Diveq,
    Dot, Comma, Semcol, Col, Dbcol, Bsl, Dbbsl, Dbfsl, Hash, Opb, Clb, Opclb, Clclb,
    Huh,
} Token;

const char *keywords[] = {
    "function", "for", "do", "if", "orif", "otherwise", "try", "while", "return", "public", "private",
    "int", "string", "float", "bool", "Null", "any", "True", "False", "module",
};

const char *operators[] = {
    "+", "-", "/", "*", "=", "=/=", "==", "+=", "-=", "/=", "*=", "<",
    ">", "<=", ">=", "%", "&", "||", "!"
};

const char *punctuation[] = {
    ".", ",", ";", ":", "::", "\\", "\\\\", "//", "#", "(", ")", "{", "}",
};

#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))
#define NUM_OPERATORS (sizeof(operators) / sizeof(operators[0]))

typedef struct {
    Token type;
    char *value;
    int line;
} MToken;

typedef struct {
    MToken *tokens;
    int count;
    int capacity;
} TokenList;

Token classifyKeyword(const char *str) {
    if (strcmp(str, "function") == 0) return Fn;
    if (strcmp(str, "public") == 0) return Pub;
    if (strcmp(str, "private") == 0) return Loc;
    if (strcmp(str, "if") == 0) return If;
    if (strcmp(str, "otherwise") == 0) return Else;
    if (strcmp(str, "return") == 0) return Return;
    if (strcmp(str, "orif") == 0) return Orif;
    if (strcmp(str, "try") == 0) return Try;
    if (strcmp(str, "while") == 0) return While;
    if (strcmp(str, "do") == 0) return Do;
    if (strcmp(str, "string") == 0) return String;
    if (strcmp(str, "int") == 0) return Int;
    if (strcmp(str, "float") == 0) return Float;
    if (strcmp(str, "bool") == 0) return Bool;
    if (strcmp(str, "True") == 0) return True;
    if (strcmp(str, "False") == 0) return False;
    if (strcmp(str, "Null") == 0) return Null;
    if (strcmp(str, "module") == 0) return Module;
    if (strcmp(str, "var") == 0) return Var;
    return Huh;
}

Token classifyOperator(const char *str, int length) {
    if (length == 3 && strncmp(str, "=/=", 3) == 0) return Ne;
    if (length == 2) {
        if (strncmp(str, "==", 2) == 0) return Eqeq;
        if (strncmp(str, "<=", 2) == 0) return Le;
        if (strncmp(str, ">=", 2) == 0) return Ge;
        if (strncmp(str, "*=", 2) == 0) return Muleq;
        if (strncmp(str, "+=", 2) == 0) return Muleq;
        if (strncmp(str, "-=", 2) == 0) return Sub;
        if (strncmp(str, "/=", 2) == 0) return Diveq;
        if (strncmp(str, "||", 2) == 0) return Or;
    }
    if (length == 1) {
        if (*str == '+') return Add;
        if (*str == '-') return Sub;
        if (*str == '*') return Mul;
        if (*str == '/') return Div;
        if (*str == '=') return Eq;
        if (*str == '<') return Lt;
        if (*str == '>') return Gt;
        if (*str == '!') return Not;
        if (*str == '%') return Mod;
        if (*str == '&') return And;
    }
    return Huh;
}

Token classifyPunctuation(const char *str, int length) {
    if (length == 2) {
        if (strncmp(str, "\\\\", 2) == 0) return Dbbsl;
        if (strncmp(str, "//", 2) == 0) return Dbfsl;
        if (strncmp(str, "::", 2) == 0) return Dbcol;
    }
    if (length == 1) {
        if (*str == ',') return Comma;
        if (*str == '.') return Dot;
        if (*str == ';') return Semcol;
        if (*str == ':') return Col;
        if (*str == '\\') return Bsl;
        if (*str == '#') return Hash;
        if (*str == '(') return Opb;
        if (*str == ')') return Clb;
        if (*str == '{') return Opclb;
        if (*str == '}') return Clclb;
    }
    return Huh;
}
