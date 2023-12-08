#include "defs.h"
#include "decl.h"
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

char *tokstr[] = {"+", "-", "*", "/", "inlit"};

static void scanfile() {
  struct token T;

  while (scan(&T)) {
    printf("Token %s", tokstr[T.token]);
    if (T_INTLIT == T.token) {
      printf(", value %d", T.intvalue);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if (2 != argc) {
    usage("argv[0]");
  }

  init();
  if (NULL == (Infile = fopen(argv[1], "r"))) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  scanfile();
  exit(0);
}
