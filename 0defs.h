#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTLEN 512
#define NSYMBOLS 1024
#define NOREG -1 // Use NOREG when the AST generation
#define NOLABEL	 0		// Use NOLABEL when we have no label to

#define msg() printf("%s()<%d>: %d\n", __FUNCTION__, __LINE__, Count++)
#define msg_s(s) printf("%s()<%d>: %s\n", __FUNCTION__, __LINE__, s)
#define msg_op(op)                                                             \
  printf("%s()<%d>: op: %s\n", __FUNCTION__, __LINE__, op_str[op - 1])
#define msg_int(i) printf("%s()<%d>: i: %d\n", __FUNCTION__, __LINE__, i)
#define msg_ptype(ptype)                                                       \
  printf("%s()<%d>: ptype: %d\n", __FUNCTION__, __LINE__, ptype)

enum {
  // T for token
  T_EOF,

  T_ASSIGN,
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
  T_IDENT,
  T_LBRACE,
  T_RBRACE,
  T_LPAREN,
  T_RPAREN,
  T_INT,
  T_IF,
  T_ELSE,
  T_WHILE,
  T_FOR,
  T_VOID,
  T_CHAR,
  T_LONG,
  T_RETURN,
  T_AMPER,
  T_LOGAND,
  T_COMMA,
  T_LBRACKET,
  T_RBRACKET,
};

struct token {
  int token;
  int intvalue;
};

enum {
  // A for abstract
  A_ASSIGN = 1,
  A_ADD,
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
  A_GLUE,
  A_IF,
  A_WHILE,
  A_FUNCTION,
  A_WIDEN,
  A_RETURN,
  A_FUNCCALL,
  A_DEREF,
  A_ADDR,
  A_SCALE,

  A_BUTT,
};

struct ASTnode {
  int op;
  int type;
  int rvalue;
  struct ASTnode *left;
  struct ASTnode *mid;
  struct ASTnode *right;
  union {
    int intvalue;
    int id;
    int size;
  } v;
};

enum {
  // S for structural
  S_VARIABLE,
  S_FUNCTION,
  S_ARRAY,
};

struct symtable {
  char *name;
  int type;  // primitive type
  int stype; // structural type
  int endlabel;
  int size;
};

enum {
  // P for primitive
  P_NONE,
  P_VOID,
  P_CHAR,
  P_INT,
  P_LONG,
  P_VOIDPTR,
  P_CHARPTR,
  P_INTPTR,
  P_LONGPTR,
};
