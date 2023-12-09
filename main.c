#include "1defs.h"
#include "decl.h"
#include <stdio.h>
#define extern_
#include "data.h"
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
  struct ASTnode *n;

  if (2 != argc) {
    usage("argv[0]");
  }

  init();
  if (NULL == (Infile = fopen(argv[1], "r"))) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  scan(&Token);
  n = binexpr();
  printf("%d\n", interpretAST(n));
  exit(0);
}
