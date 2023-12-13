#include "1defs.h"
#include "decl.h"
#include "data.h"
#include <stdio.h>

void var_declaration() {
  match(T_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}
