#include "0defs.h"
#include "1decl.h"
#include "2data.h"

static struct ASTnode *single_statement(void);

static struct ASTnode *return_statement() {
  struct ASTnode *tree;
  int returntype, functype;

  if (P_VOID == Gsym[Functionid].type) {
    fatal("Can't return from a void function");
  }

  match(T_RETURN, "return");
  lparen();

  tree = binexpr(0);

  returntype = tree->type;
  functype = Gsym[Functionid].type;
  if (!type_compatible(&returntype, &functype, 1)) {
    fatal("Incompatible types");
  }

  if (returntype) {
    tree = mkastunary(returntype, Gsym[Functionid].type, tree, 0);
  }

  tree = mkastunary(A_RETURN, P_NONE, tree, 0);

  rparen();

  return (tree);
}

struct ASTnode *print_statement() {
  struct ASTnode *tree;
  int lefttype, righttype;

  match(T_PRINT, "print");
  tree = binexpr(0);

  lefttype = P_INT;
  righttype = tree->type;

  if (!type_compatible(&lefttype, &righttype, 0)) {
    fatal("Incompatible types");
  }

  if (righttype) {
    tree = mkastunary(righttype, P_INT, tree, 0);
  }

  tree = mkastunary(A_PRINT, P_NONE, tree, 0);
  return (tree);
}

struct ASTnode *assignment_statement() {
  struct ASTnode *left, *right, *tree;
  int lefttype, righttype;
  int id;

  ident();

  if (T_LPAREN == Token.token) {
    return (funccall());
  }

  if (-1 == (id = findglob(Text))) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, Gsym[id].type, id);

  match(T_ASSIGN, "=");

  left = binexpr(0);

  lefttype = left->type;
  righttype = right->type;
  if (!type_compatible(&lefttype, &righttype, 1)) {
    fatal("Incompatible types");
  }

  if (lefttype) {
    left = mkastunary(lefttype, right->type, left, 0);
  }
  tree = mkastnode(A_ASSIGN, P_INT, left, NULL, right, 0);

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
  return mkastnode(A_IF, P_NONE, conAST, trueAST, falseAST, 0);
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

  return mkastnode(A_WHILE, P_NONE, condAST, NULL, bodyAST, 0);
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

  tree = mkastnode(A_GLUE, P_NONE, bodyAST, NULL, postopAST, 0);
  tree = mkastnode(A_WHILE, P_NONE, condAST, NULL, tree, 0);
  return mkastnode(A_GLUE, P_NONE, preopAST, NULL, tree, 0);
}

struct ASTnode *single_statement() {
  switch (Token.token) {
  case T_PRINT:
    return print_statement();
  case T_INT:
  case T_CHAR:
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
  case T_RETURN:
    return return_statement();
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

    if (NULL != tree && (A_PRINT == tree->op || A_ASSIGN == tree->op ||
                         A_RETURN == tree->op || A_FUNCCALL == tree->op)) {
      semi();
    }

    if (tree) {
      if (NULL == left) {
        left = tree;
      } else {
        left = mkastnode(A_GLUE, P_NONE, left, NULL, tree, 0);
      }
    }

    if (T_RBRACE == Token.token) {
      rbrace();
      return left;
    }
  }
}
