#include "defs.h"
#include "data.h"
#include "decl.h"

static int next() {
  int c;
  if (Putback) {
    c = Putback;
    Putback = 0;
    return c;
  }

  c = fgetc(Infile);
  if ('\n' == c) {
    Line++;
  }
  return c;
}

static void putbakc(int c) { Putback = c; }

static int skip() {
  int c;

  c = next();
  while (' ' == c || '\n' == c || '\t' == c || '\r' == c || '\f' == c) {
    c = next();
  }
  return c;
}

static int chrpos(char *str, int c) {
  char *p;
  p = strchr(str, c);
  return p ? p - str : -1;
}

static int scanint(int c) {
  int k, val = 0;

  while((k = chrpos("0123456789", c)) >= 0){
    val = val * 10 + k;
    c = next();
  }

  putbakc(c);
  return val;
}

int scan(struct token *t) {
  int c;

  c = skip();
  switch (c) {
  case EOF:
    return 0;
  case '+':
    t->token = T_PLUS;
    break;
  case '-':
    t->token = T_MINUS;
    break;
  case '*':
    t->token = T_STAR;
    break;
  case '/':
    t->token = T_SLASH;
    break;
  default:
    if (isdigit(c)) {
      t->token = T_INTLIT;
      t->intvalue = scanint(c);
      break;
    }
    printf("Unregconised character %c on line %d\n", c, Line);
    exit(1);
  }
  return 1;
}
