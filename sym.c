#include "0defs.h"
#include "1decl.h"
#include "2data.h"

int findglob(char *s) {
  int loop;

  for (loop = 0; loop < Globs; loop++) {
    if (*s == *Gsym[loop].name && !strcmp(s, Gsym[loop].name)) {
      return loop;
    }
  }

  return -1;
}

static int newglob() {
  int p;

  if ((p = Globs++) >= NSYMBOLS) {
    fatal("Too many global symbols");
  }

  return p;
}

int addglob(char *name, int type, int stype, int endlabel) {
  int y;

  if (-1 != (y = findglob(name))) {
    return y;
  }

  y = newglob();
  Gsym[y].name = strdup(name);
  Gsym[y].type = type;
  Gsym[y].stype = stype;
  Gsym[y].endlabel = endlabel;
  return y;
}
