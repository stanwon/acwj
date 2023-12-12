#include "1defs.h"
#include "data.h"
#include "decl.h"

void match(int t, char *what) {
  if (t == Token.token) {
    scan(&Token);
  } else {
    printf("%s expected on line %d\n", what, Line);
    exit(1);
  }
}

void semi() { match(T_SEMI, ";"); }

void statements() {
  struct ASTnode *tree;
  int reg;

  while (1) {
    match(T_PRINT, "print");

    tree = binexpr(0);
    reg = genAST(tree);
    genprintint(reg);
    genfreeregs();

    semi();
    if (T_EOF == Token.token) {
      return;
    }
  }
}
