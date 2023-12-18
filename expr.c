#include "1defs.h"
#include "data.h"
#include "decl.h"

static int OpPrec[] = {0, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40};

static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (0 == prec) {
    fatald("Syntax error, token", tokentype);
  }
  return prec;
}

int arithop(int tokentype) {
  if (tokentype > T_EOF && tokentype < T_INTLIT)
    return tokentype;
  fatald("Syntax error, token", tokentype);
}

static struct ASTnode *primary() {
  struct ASTnode *n;
  int id;

  switch (Token.token) {
  case T_INTLIT:
    n = mkastleaf(A_INTLIT, Token.intvalue);
    break;
  case T_IDENT:
    id = findglob(Text);
    if (-1 == id) {
      fatals("Unknown variable", Text);
    }
    n = mkastleaf(A_IDENT, id);
    break;
  default:
    fatald("Syntax error, token", Token.token);
  }

  scan(&Token);
  return n;
}

struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (T_SEMI == tokentype || T_RPAREN == tokentype) {
    return left;
  }

  while (op_precedence(tokentype) > ptp) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, NULL, right, 0);

    tokentype = Token.token;
    if (T_SEMI == tokentype || T_RPAREN == tokentype) {
      return left;
    }
  }
  return left;
}
