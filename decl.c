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
    if (T_LBRACKET == Token.token) {
      scan(&Token);

      if (T_INTLIT == Token.token) {
        id = addglob(Text, pointer_to(type), S_ARRAY, 0, Token.intvalue);
        genglobsym(id);
      }

      scan(&Token);
      match(T_RBRACKET, "]");
    } else {
      id = addglob(Text, type, S_VARIABLE, 0, 1);
      genglobsym(id);
    }

    if (Token.token == T_SEMI) {
      scan(&Token);
      return;
    }

    if (Token.token == T_COMMA) {
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
  nameslot = addglob(Text, type, S_FUNCTION, endlabel, 0);
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
