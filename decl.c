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
  case T_LONG:
    type = P_LONG;
    break;
  default:
    fatald("Illegal type, token", Token.token);
  }

  while (1) {
    scan(&Token);
    if (T_STAR != Token.token) {
      break;
    }
    type = pointer_to(type);
  }

  return type;
}

void global_declarations() {
  struct ASTnode *tree;
  int type;

  while (1) {
    type = parse_type();
    ident();

    if (T_LPAREN == Token.token) {
      tree = function_declaration(type);
      if (O_dumpAST) {
        dumpAST(tree, NOLABEL, 0);
        fprintf(stdout, "\n\n");
      }
      genAST(tree, NOREG, 0);
    } else {
      var_declaration(type);
    }

    if (T_EOF == Token.token) {
      break;
    }
  }
}

void var_declaration(int type) {
  int id;

  while (1) {
    id = addglob(Text, type, S_VARIABLE, 0);
    genglobsym(id);

    if (T_SEMI == Token.token) {
      scan(&Token);
      return;
    }

    if (T_COMMA == Token.token) {
      scan(&Token);
      ident();
      continue;
    }
    fatal("Missing , or ; after identifier");
  }
}

struct ASTnode *function_declaration(int type) {
  struct ASTnode *tree, *finalstmt;
  int nameslot, endlabel;

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
