#include "0defs.h"
#include "1decl.h"
#define extern_
#include "2data.h"
#undef extern_
#include <errno.h>

char *op_str[] = {"A_ASSIGN",   "A_ADD",   "A_SUBTRACT", "A_MULTIPLY",
                  "A_DIVIDE",   "A_EQ",    "A_NE",       "A_LT",
                  "A_GT",       "A_LE",    "A_GE",       "A_INTLIT",
                  "A_IDENT",    "A_GLUE",  "A_IF",       "A_WHILE",
                  "A_FUNCTION", "A_WIDEN", "A_RETURN",   "A_FUNCCALL",
                  "A_DEREF",    "A_ADDR",  "A_SCALE"};

static void init() {
  Line = 1;
  Putback = '\n';
  Globs = 0;
  O_dumpAST = 0;
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s [-T] infile\n", prog);
  exit(1);
}

int main(int argc, char *argv[]) {
  struct ASTnode *tree = NULL;
  int i;

  init();

  for (i = 1; i < argc; i++) {
    if (*argv[i] != '-')
      break;
    for (int j = 1; argv[i][j]; j++) {
      switch (argv[i][j]) {
      case 'T':
        O_dumpAST = 1;
        break;
      default:
        usage(argv[0]);
      }
    }
  }

  if (i >= argc)
    usage(argv[0]);

  if (NULL == (Infile = fopen(argv[i], "r"))) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[i], strerror(errno));
    exit(1);
  }

  if (NULL == (Outfile = fopen("out.s", "w"))) {
    fprintf(stderr, "Unable to open out.s: %s\n", strerror(errno));
    exit(1);
  }

  addglob("printint", P_CHAR, S_FUNCTION, 0);

  scan(&Token);
  genpreamble();
  global_declarations();
  genpostamble();
  fclose(Outfile);
  exit(0);
}
