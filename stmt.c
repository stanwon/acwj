#include "1defs.h"
#include "data.h"
#include "decl.h"
#include <stdio.h>

void print_statement() {
  struct ASTnode *tree;
  int reg;
  match(T_PRINT, "print");

  tree = binexpr(0);

  reg = genAST(tree, -1);
  genprintint(reg);
  genfreeregs();
  semi();
}

void assignment_statement() {
  struct ASTnode *left, *right, *tree;
  int id;
  ident();

  if (-1 == (id = findglob(Text))) {
    fatals("Undeclared variable", Text);
  }

  right = mkastleaf(A_LVIDENT, id);

  match(T_EQUALS, "=");

  left = binexpr(0);

  tree = mkastnode(A_ASSIGN, left, right, 0);
  genAST(tree, -1);
  genfreeregs();
  semi();
}

void statements() {
  struct ASTnode *tree;
  int reg;

  while (1) {
    switch (Token.token) {
    case T_PRINT:
      print_statement();
      break;
    case T_INT:
      var_declaration();
      break;
    case T_IDENT:
      assignment_statement();
      break;
    case T_EOF:
      return;
    default:
      fatald("Syntax error, token", Token.token);
    }
  }
}
