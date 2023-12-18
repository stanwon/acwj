#include "1defs.h"
#include "data.h"
#include "decl.h"

static struct ASTnode *single_statement(void);

struct ASTnode *print_statement() {
  struct ASTnode *tree;

  match(T_PRINT, "print");
  tree = binexpr(0);
  tree = mkastunary(A_PRINT, tree, 0);

  return tree;
}

struct ASTnode *assignment_statement() {
  struct ASTnode *left, *right, *tree;
  int id;

  ident();

  if (-1 == (id = findglob(Text))) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, id);

  match(T_ASSIGN, "=");

  left = binexpr(0);

  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  return tree;
}

struct ASTnode *if_statement() {
  struct ASTnode *conAST, *trueAST, *falseAST = NULL;

  match(T_IF, "if");
  lparen();

  conAST = binexpr(0);
  if (conAST->op < T_EQ || conAST->op > T_GE) {
    fatal("Bad comparison operator");
  }

  rparen();
  trueAST = compound_statement();
  if (T_ELSE == Token.token) {
    scan(&Token);
    falseAST = compound_statement();
  }
  return mkastnode(A_IF, conAST, trueAST, falseAST, 0);
}

struct ASTnode *while_statement() {
  struct ASTnode *condAST, *bodyAST;

  match(T_WHILE, "while");
  lparen();

  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE) {
    fatal("Bad comparison operator");
  }
  rparen();

  bodyAST = compound_statement();

  return mkastnode(A_WHILE, condAST, NULL, bodyAST, 0);
}

struct ASTnode *for_statement() {
  struct ASTnode *condAST, *bodyAST;
  struct ASTnode *preopAST, *postopAST;
  struct ASTnode *tree;

  match(T_FOR, "for");
  lparen();

  preopAST = single_statement();
  semi();

  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE) {
    fatal("Bad comparison operator");
  }
  semi();

  postopAST = single_statement();
  rparen();

  bodyAST = compound_statement();

  tree = mkastnode(A_GLUE, bodyAST, NULL, postopAST, 0);
  tree = mkastnode(A_WHILE, condAST, NULL, tree, 0);
  return mkastnode(A_GLUE, preopAST, NULL, tree, 0);
}

struct ASTnode *single_statement() {
  switch (Token.token) {
  case T_PRINT:
    return print_statement();
  case T_INT:
    var_declaration();
    return NULL;
  case T_IDENT:
    return assignment_statement();
  case T_IF:
    return if_statement();
  case T_WHILE:
    return while_statement();
  case T_FOR:
    return for_statement();
  default:
    fatald("Syntax error, token", Token.token);
  }
}

struct ASTnode *compound_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  lbrace();

  while (1) {
    tree = single_statement();

    if (NULL != tree && (A_PRINT == tree->op || A_ASSIGN == tree->op)) {
      semi();
    }

    if (tree) {
      if (NULL == left) {
        left = tree;
      } else {
        left = mkastnode(A_GLUE, left, NULL, tree, 0);
      }
    }

    if (T_RBRACE == Token.token) {
      rbrace();
      return left;
    }
  }
}
