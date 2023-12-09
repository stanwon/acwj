#include "1defs.h"
#include "data.h"
#include "decl.h"
#include <stdio.h>

int arithop(int token) {
  switch (token) {
  case T_PLUS:
    return A_ADD;
  case T_MINUS:
    return A_SUBTRACT;
  case T_STAR:
    return A_MULTIPLY;
  case T_SLASH:
    return A_DIVIDE;
  default:
    fprintf(stderr, "unknown token in arithop() on line%d\n", Line);
    exit(1);
  }
}

static struct ASTnode *primary() {
  struct ASTnode *n;
  switch (Token.token) {
  case T_INTLIT:
    n = mkastleaf(A_INTLIT, Token.intvalue);
    scan(&Token);
    return n;
  default:
    fprintf(stderr, "syntax error on line %d\n", Line);
    exit(1);
  }
}

struct ASTnode *binexpr() {
  struct ASTnode *n, *left, *right;
  int nodetype;

  left = primary();
  if (T_EOF == Token.token) {
    return left;
  }

  nodetype = arithop(Token.token);
  scan(&Token);
  right = binexpr();
  n = mkastnode(nodetype, left, right, 0);
  return n;
}
