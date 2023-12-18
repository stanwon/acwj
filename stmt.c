#include "1defs.h"
#include "data.h"
#include "decl.h"
#include <stdio.h>

struct ASTnode *print_statement() {
  struct ASTnode *tree;

  match(T_PRINT, "print");
  tree = binexpr(0);
  tree = mkastunary(A_PRINT, tree, 0);
  semi();

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
  semi();

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

struct ASTnode *compound_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  lbrace();

  while (1) {
    switch (Token.token) {
    case T_PRINT:
      tree = print_statement();
      break;
    case T_INT:
      var_declaration();
      tree = NULL;
      break;
    case T_IDENT:
      tree = assignment_statement();
      break;
    case T_IF:
      tree = if_statement();
      break;
    case T_RBRACE:
      rbrace();
      return left;
    default:
      fatald("Syntax error, token", Token.token);
    }

    if (tree) {
      if (NULL == left) {
        left = tree;
      } else {
        left = mkastnode(A_GLUE, left, NULL, tree, 0);
      }
    }
  }
}
