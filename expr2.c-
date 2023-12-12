#include "1defs.h"
#include "data.h"
#include "decl.h"
#include <stdio.h>

static struct ASTnode *primary() {
  struct ASTnode *n;
  switch (Token.token) {
  case T_INTLIT:
    n = mkastleaf(A_INTLIT, Token.intvalue);
    scan(&Token);
    return n;
  default:
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, Token.token);
    exit(1);
  }
}

static int arithop(int tok) {
  switch (tok) {
  case T_PLUS:
    return A_ADD;
  case T_MINUS:
    return A_SUBTRACT;
  case T_STAR:
    return A_MULTIPLY;
  case T_SLASH:
    return A_DIVIDE;
  default:
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, Token.token);
    exit(1);
  }
}

struct ASTnode *additive_expr();

struct ASTnode *multiplicative_expr() {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (T_EOF == tokentype) {
    return left;
  }

  while (T_STAR == tokentype || T_SLASH == tokentype) {
    scan(&Token);
    right = primary();

    left = mkastnode(arithop(tokentype), left, right, 0);
    tokentype = Token.token;
    if (T_EOF == tokentype) {
      break;
    }
  }

  return left;
}

struct ASTnode *additive_expr() {
  struct ASTnode *left, *right;
  int tokentype;

  left = multiplicative_expr();

  tokentype = Token.token;
  if (T_EOF == tokentype) {
    return left;
  }

  while (1) {
    scan(&Token);
    right = multiplicative_expr();
    left = mkastnode(arithop(tokentype), left, right, 0);
    tokentype = Token.token;
    if (T_EOF == tokentype) {
      break;
    }
  }

  return left;
}

struct ASTnode *binexpr() { return additive_expr(); }
