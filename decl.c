#include "0defs.h"
#include "1decl.h"
#include "2data.h"

int parse_type(int type) {
  switch (type) {
  case T_CHAR:
    return P_CHAR;
  case T_INT:
    return P_INT;
  case T_VOID:
    return P_VOID;
  default:
    fatald("Illegal type, token", type);
  }
}

void var_declaration() {
  int id, type;

  type = parse_type(Token.token);
  scan(&Token);
  ident();

  id = addglob(Text, type, S_VARIABLE);
  genglobsym(id);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree;
  int nameslot, type;

  type = parse_type(Token.token);
  scan(&Token);
  ident();
  nameslot = addglob(Text, type, S_FUNCTION);
  Functionid = nameslot;
  lparen();
  rparen();

  tree = compound_statement();

  return mkastunary(A_FUNCTION, type, tree, nameslot);
}
