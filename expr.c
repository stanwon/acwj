#include "1defs.h"
#include "data.h"
#include "decl.h"

static int OpPrec[] = {0, 10, 10, 20, 20, 0};

static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (0 == prec) {
    fprintf(stderr, "syntax error on line %d, tokentype %d\n", Line, tokentype);
    exit(1);
  }
  return prec;
}

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

struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (T_EOF == tokentype) {
    return left;
  }

  while (op_precedence(tokentype) > ptp) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = Token.token;
    if(T_EOF == tokentype){
      break;
    }
  }

  return left;
}
