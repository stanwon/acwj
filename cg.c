#include "0defs.h"
#include "1decl.h"
#include "2data.h"

static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};
static char *breglist[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};
static char *dreglist[4] = {"%r8d", "%r9d", "%r10d", "%r11d"};
static char *cmplist[] = {"sete", "setne", "setl", "setg", "setle", "setge"};
static char *invcmplist[] = {"jne", "je", "jge", "jle", "jg", "jl"};

void freeall_registers() {
  freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

static int alloc_register() {
  int loop;
  for (loop = 0; loop < 4; loop++) {
    if (freereg[loop]) {
      freereg[loop] = 0;
      return loop;
    }
  }

  fatal("Out of registers");
}

static void free_register(int reg) {
  if (0 != freereg[reg]) {
    fatald("Error trying to free register", reg);
  }
  freereg[reg] = 1;
}

void cgpreamble() {
  freeall_registers();
  fputs("\t.text\n", Outfile);
}

void cgfuncpreamble(int id) {
  char *name = Gsym[id].name;
  fprintf(Outfile,
          "\t.text\n"
          "\t.globl\t%s\n"
          "\t.type\t%s, @function\n"
          "%s:\n"
          "\tpushq\t%%rbp\n"
          "\tmovq\t%%rsp, %%rbp\n",
          name, name, name);
}

void cgpostamble() {}

void cgfuncpostamble(int id) {
  cglabel(Gsym[id].endlabel);
  fputs("\tpopq	%rbp\n"
        "\tret\n",
        Outfile);
}

int cgloadint(int value, int type) {
  int r = alloc_register();
  fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
  return r;
}

int cgloadglob(int id) {
  int r = alloc_register();

  switch (Gsym[id].type) {
  case P_CHAR:
    fprintf(Outfile, "\tmovzbq\t%s(\%%rip), %s\n", Gsym[id].name, reglist[r]);
    break;
  case P_INT:
    fprintf(Outfile, "\tmovzbl\t%s(\%%rip), %s\n", Gsym[id].name, dreglist[r]);
    break;
  case P_LONG:
  case P_CHARPTR:
  case P_INTPTR:
  case P_LONGPTR:
    fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", Gsym[id].name, reglist[r]);
    break;
  default:
    fatald("Bad type in cgloadglob", Gsym[id].type);
  }

  return r;
}

int cgadd(int r1, int r2) {
  fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return r2;
}

int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return r1;
}

int cgmul(int r1, int r2) {
  fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return r2;
}

int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  free_register(r2);
  return (r1);
}

void cgprintint(int r) {
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\tprintint\n");
  free_register(r);
}

int cgstorglob(int r, int id) {
  switch (Gsym[id].type) {
  case P_CHAR:
    fprintf(Outfile, "\tmovb\t%s, %s(\%%rip)\n", breglist[r], Gsym[id].name);
    break;
  case P_INT:
    fprintf(Outfile, "\tmovl\t%s, %s(\%%rip)\n", dreglist[r], Gsym[id].name);
    break;
  case P_LONG:
  case P_CHARPTR:
  case P_INTPTR:
  case P_LONGPTR:
    fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], Gsym[id].name);
    break;
  default:
    fatald("Bad type in cgstorglob", Gsym[id].type);
  }
  return (r);
}

void cgglobsym(int id) {
  int typesize;

  typesize = cgprimsize(Gsym[id].type);
  fprintf(Outfile,
          "\t.data\n"
          "\t.globl\t%s\n",
          Gsym[id].name);
  fprintf(Outfile, "%s:", Gsym[id].name);
  for (int i = 0; i < Gsym[id].size; i++) {
    switch (typesize) {
    case 1:
      fprintf(Outfile, "\t.byte\t0\n");
      break;
    case 4:
      fprintf(Outfile, "\t.long\t0\n");
      break;
    case 8:
      fprintf(Outfile, "\t.quad\t0\n");
      break;
    default:
      fatald("Unknown typesize in cgglobsym: ", typesize);
    }
  }
}

int cgcompare_and_set(int ASTop, int r1, int r2) {
  if (ASTop < A_EQ || ASTop > A_GE) {
    fatal("Bad ASTop in cgcompare_and_set()");
  }

  fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
  fprintf(Outfile, "\t%s\t%s\n", cmplist[ASTop - A_EQ], breglist[r2]);
  fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r2], reglist[r2]);
  free_register(r1);
  return (r2);
}

int cgcompare_and_jump(int ASTop, int r1, int r2, int label) {
  if (ASTop < A_EQ || ASTop > A_GE)
    fatal("Bad ASTop in cgcompare_and_set()");

  fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
  fprintf(Outfile, "\t%s\tL%d\n", invcmplist[ASTop - A_EQ], label);
  freeall_registers();
  return (NOREG);
}

void cglabel(int l) { fprintf(Outfile, "L%d:\n", l); }

void cgjump(int l) { fprintf(Outfile, "\tjmp\tL%d\n", l); }

int cgwiden(int r, int oldtype, int newtype) { return (r); }

static int psize[] = {0, 0, 1, 4, 8, 8, 8, 8, 8};
int cgprimsize(int type) {
  if (P_NONE > type || P_LONGPTR < type) {
    fatal("Bad type in cgprimsize()");
  }
  return psize[type];
}

int cgcall(int r, int id) {
  int outr = alloc_register();
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\t%s\n", Gsym[id].name);
  fprintf(Outfile, "\tmovq\t%%rax, %s\n", reglist[outr]);
  free_register(r);
  return (outr);
}

int cgreturn(int r, int id) {
  switch (Gsym[id].type) {
  case P_CHAR:
    fprintf(Outfile, "\tmovzbl\t%s, %%eax\n", breglist[r]);
    break;
  case P_INT:
    fprintf(Outfile, "\tmovl\t%s, %%eax\n", dreglist[r]);
    break;
  case P_LONG:
    fprintf(Outfile, "\tmovq\t%s, %%rax\n", reglist[r]);
    break;
  default:
    fatald("Bad function type in cgreturn", Gsym[id].type);
  }
  cgjump(Gsym[id].endlabel);
}

int cgaddress(int id) {
  int r = alloc_register();

  fprintf(Outfile, "\tleaq\t%s(%%rip), %s\n", Gsym[id].name, reglist[r]);
  return (r);
}

int cgderef(int r, int type) {
  switch (type) {
  case P_CHARPTR:
    fprintf(Outfile, "\tmovzbq\t(%s), %s\n", reglist[r], reglist[r]);
    break;
  case P_INTPTR:
  case P_LONGPTR:
    fprintf(Outfile, "\tmovq\t(%s), %s\n", reglist[r], reglist[r]);
    break;
  }
  return (r);
}

int cgshlconst(int r, int val) {
  fprintf(Outfile, "\tsalq\t$%d, %s\n", val, reglist[r]);
  return (r);
}

int cgstorderef(int r1, int r2, int type) {
  switch (type) {
  case P_CHAR:
    fprintf(Outfile, "\tmovb\t%s, (%s)\n", breglist[r1], reglist[r2]);
    break;
  case P_INT:
    fprintf(Outfile, "\tmovq\t%s, (%s)\n", reglist[r1], reglist[r2]);
    break;
  case P_LONG:
    fprintf(Outfile, "\tmovq\t%s, (%s)\n", reglist[r1], reglist[r2]);
    break;
  default:
    fatald("Can't cgstoderef on type:", type);
  }
  return (r1);
}

void cgglobstr(int l, char *str) {
  char *cptr;

  cglabel(l);
  for (cptr = str; *cptr; cptr++) {
    fprintf(Outfile, "\t.byte\t%d\n", *cptr);
  }
  fprintf(Outfile, "\t.byte\t0\n");
}

int cgloadglobstr(int id) {
  int r = alloc_register();

  fprintf(Outfile, "\tleaq\tL%d(\%%rip), %s\n", id, reglist[r]);
  return (r);
}
