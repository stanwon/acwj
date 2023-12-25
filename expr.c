#include "0defs.h"
#include "1decl.h"
#include "2data.h"

static int OpPrec[] = {0, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40};

struct ASTnode *funccall() {
  struct ASTnode *tree;
  int id;

  if (-1 == (id = findglob(Text))) {
    fatals("Undeclared function", Text);
  }

  lparen();
  tree = binexpr(0);
  tree = mkastunary(A_FUNCCALL, Gsym[id].type, tree, id);
  rparen();
  return tree;
}
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
    if (Token.intvalue >= 0 && Token.intvalue < 256) {
      n = mkastleaf(A_INTLIT, P_CHAR, Token.intvalue);
    } else {
      n = mkastleaf(A_INTLIT, P_INT, Token.intvalue);
    }
    break;
  case T_IDENT:
    scan(&Token);
    if (T_LPAREN == Token.token) {
      return funccall();
    }

    reject_token(&Token);

    id = findglob(Text);
    if (-1 == id) {
      fatals("Unknown variable", Text);
    }
    n = mkastleaf(A_IDENT, Gsym[id].type, id);
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
  int lefttype, righttype;

  left = primary();

  tokentype = Token.token;
  if (T_SEMI == tokentype || T_RPAREN == tokentype) {
    return left;
  }

  while (op_precedence(tokentype) > ptp) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    lefttype = left->type;
    righttype = right->type;
    if (!type_compatible(&lefttype, &righttype, 0)) {
      fatal("Incompatible types");
    }

    if (lefttype) {
      left = mkastunary(lefttype, right->type, left, 0);
    }
    if (righttype) {
      right = mkastunary(righttype, left->type, right, 0);
    }

    left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

    tokentype = Token.token;
    if (T_SEMI == tokentype || T_RPAREN == tokentype) {
      return left;
    }
  }
  return left;
}
