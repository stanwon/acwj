#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTLEN 512
#define NSYMBOLS 1024

enum {
  // T for token
  T_EOF,

  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,

  T_EQ,
  T_NE,
  T_LT,
  T_GT,
  T_LE,
  T_GE,

  T_INTLIT,

  T_SEMI,
  T_ASSIGN,
  T_IDENT,

  T_PRINT,
  T_INT,
};

struct token {
  int token;
  int intvalue;
};

enum {
  // A for abstract
  A_ADD = 1,
  A_SUBTRACT,
  A_MULTIPLY,
  A_DIVIDE,

  A_EQ,
  A_NE,
  A_LT,
  A_GT,
  A_LE,
  A_GE,

  A_INTLIT,

  A_IDENT,
  A_LVIDENT,
  A_ASSIGN,
};

struct ASTnode {
  int op;
  struct ASTnode *left;
  struct ASTnode *right;
  union {
    int intvalue;
    int id;
  } v;
};

struct symtable {
  char *name;
};
