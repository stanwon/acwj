#include "0defs.h"
#include "1decl.h"
#define extern_
#include "2data.h"
#undef extern_
#include <errno.h>

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

  if (NULL == (Infile = fopen(argv[1], "r"))) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  if (NULL == (Outfile = fopen("out.s", "w"))) {
    fprintf(stderr, "Unable to open out.s: %s\n", strerror(errno));
    exit(1);
  }

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
