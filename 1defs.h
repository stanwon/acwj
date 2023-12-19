#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTLEN 512
#define NSYMBOLS 1024
#define NOREG -1 // Use NOREG when the AST generation

#define msg() printf("%s()<%d>: %d\n", __FUNCTION__, __LINE__, Count++)
#define msg_op(op) printf("%s()<%d>: op: %d\n", __FUNCTION__, __LINE__, op)

enum {
  // T for token
  T_EOF = 0,

  T_PLUS = 1,
  T_MINUS,
  T_STAR,
  T_SLASH,

  T_EQ = 5,
  T_NE,
  T_LT,
  T_GT,
  T_LE,
  T_GE,

  T_INTLIT = 11,

  T_SEMI = 12,
  T_ASSIGN,
  T_IDENT,

  T_LBRACE = 15,
  T_RBRACE,
  T_LPAREN,
  T_RPAREN,

  T_PRINT = 19,
  T_INT,
  T_IF,
  T_ELSE,
  T_WHILE,
  T_FOR,
  T_VOID,
  T_CHAR,
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

  A_EQ = 5,
  A_NE,
  A_LT,
  A_GT,
  A_LE,
  A_GE,

  A_INTLIT = 11,

  A_IDENT = 12,
  A_LVIDENT,
  A_ASSIGN,
  A_PRINT,
  A_GLUE,
  A_IF,
  A_WHILE,
  A_FUNCTION,
  A_WIDEN,
};

struct ASTnode {
  int op;
  int type;
  struct ASTnode *left;
  struct ASTnode *mid;
  struct ASTnode *right;
  union {
    int intvalue;
    int id;
  } v;
};

enum {
  // S for structural
  S_VARIABLE,
  S_FUNCTION,
};

struct symtable {
  char *name;
  int type;  // primitive type
  int stype; // structural type
};

enum {
  // P for primitive
  P_NONE,
  P_VOID,
  P_CHAR,
  P_INT,
};
