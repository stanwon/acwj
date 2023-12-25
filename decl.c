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

  id = addglob(Text, type, S_VARIABLE, 0);
  genglobsym(id);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree, *finalstmt;
  int nameslot, type, endlabel;

  type = parse_type(Token.token);
  scan(&Token);
  ident();

  endlabel  = genlabel();
  nameslot = addglob(Text, type, S_FUNCTION, endlabel);
  Functionid = nameslot;
  lparen();
  rparen();

  tree = compound_statement();

  if (P_VOID != type) {
    finalstmt = (A_GLUE == tree->op) ? tree->right : tree;
    if (NULL == finalstmt || A_RETURN != finalstmt->op)
      fatal("No return for function with non-void type");
  }
  return mkastunary(A_FUNCTION, type, tree, nameslot);
}
