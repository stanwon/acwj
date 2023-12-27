#include "0defs.h"
#include "1decl.h"
#include "2data.h"

int parse_type() {
  int type;
  switch (Token.token) {
  case T_CHAR:
    type = P_CHAR;
    break;
  case T_INT:
    type = P_INT;
    break;
  case T_VOID:
    type = P_VOID;
    break;
  default:
    fatald("Illegal type, token", Token.token);
  }

  while (1) {
    scan(&Token);
    if (T_STAR != Token.token){
      break;
    }
    type = pointer_to(type);
  }

  return type;
}

void var_declaration() {
  int id, type;

  type = parse_type();
  ident();

  id = addglob(Text, type, S_VARIABLE, 0);
  genglobsym(id);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree, *finalstmt;
  int nameslot, type, endlabel;

  type = parse_type();
  ident();

  endlabel = genlabel();
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
