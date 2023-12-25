#include "0defs.h"
#include "1decl.h"
#define extern_
#include "2data.h"
#undef extern_
#include <errno.h>

char *op_str[] = {"A_ADD",     "A_SUBTRACT", "A_MULTIPLY", "A_DIVIDE",
                  "A_EQ",      "A_NE",       "A_LT",       "A_GT",
                  "A_LE",      "A_GE",       "A_INTLIT",   "A_IDENT",
                  "A_LVIDENT", "A_ASSIGN",   "A_PRINT",    "A_GLUE",
                  "A_IF",      "A_WHILE",    "A_FUNCTION", "A_WIDEN",
                  "A_RETURN",  "A_FUNCCALL"};

static void init() {
  Line = 1;
  Putback = '\n';
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

int main(int argc, char *argv[]) {
  struct ASTnode *tree = NULL;

  if (2 != argc) {
    usage("argv[0]");
  }

  init();

  // msg_op(A_IF);

  if (NULL == (Infile = fopen(argv[1], "r"))) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  if (NULL == (Outfile = fopen("out.s", "w"))) {
    fprintf(stderr, "Unable to open out.s: %s\n", strerror(errno));
    exit(1);
  }

  addglob("printint", P_CHAR, S_FUNCTION, 0);

  scan(&Token);
  genpreamble();
  while (1) {
    tree = function_declaration();
    genAST(tree, NOREG, 0);
    if (T_EOF == Token.token) {
      break;
    }
  }

  fclose(Outfile);
  exit(0);
}
