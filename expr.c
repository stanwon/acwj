#include "0defs.h"
#include "1decl.h"
#include "2data.h"

static int OpPrec[] = {
    0,  10,        // T_EOF,  T_ASSIGN
    20, 20,        // T_PLUS, T_MINUS
    30, 30,        // T_STAR, T_SLASH
    40, 40,        // T_EQ, T_NE
    50, 50, 50, 50 // T_LT, T_GT, T_LE, T_GE
};

static int rightassoc(int tokentype) {
  if (T_ASSIGN == tokentype) {
    return 1;
  }
  return 0;
}

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

struct ASTnode *prefix() {
  struct ASTnode *tree;
  switch (Token.token) {
  case T_AMPER:
    scan(&Token);
    tree = prefix();
    if (A_IDENT != tree->op) {
      fatal("& operator must be followed by an identifier");
    }
    tree->op = A_ADDR;
    tree->type = pointer_to(tree->type);
    break;
  case T_STAR:
    scan(&Token);
    tree = prefix();
    if (A_IDENT != tree->op && A_DEREF != tree->op) {
      fatal("* operator must be followed by an identifier");
    }
    tree = mkastunary(A_DEREF, value_at(tree->type), tree, 0);
    break;
  default:
    tree = primary();
    // dumpAST(tree, NOLABEL, 0);
  }
  return tree;
}

struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  struct ASTnode *ltemp, *rtemp;
  int ASTop;
  int tokentype;

  left = prefix();

  tokentype = Token.token;
  if (T_SEMI == tokentype || T_RPAREN == tokentype) {
    left->rvalue = 1;
    return left;
  }

  while ((op_precedence(tokentype) > ptp) ||
         (rightassoc(tokentype) && op_precedence(tokentype) == ptp)) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    ASTop = arithop(tokentype);
    if (A_ASSIGN == ASTop) {
      right->rvalue = 1;
      right = modify_type(right, left->type, 0);
      if (right == NULL)
        fatal("Incompatible expression in assignment");
      ltemp = left;
      left = right;
      right = ltemp;
    } else {
      left->rvalue = 1;
      right->rvalue = 1;
      ltemp = modify_type(left, right->type, ASTop);
      rtemp = modify_type(right, left->type, ASTop);
      if (NULL == ltemp && NULL == rtemp) {
        fatal("Incompatible types in binary expression");
      }
      if (NULL != ltemp) {
        left = ltemp;
      }
      if (NULL != rtemp) {
        right = rtemp;
      }
    }

    left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

    tokentype = Token.token;
    if (T_SEMI == tokentype || T_RPAREN == tokentype) {
      return left;
    }
  }
  return left;
}
