// scan.c
int scan(struct token *t);
void reject_token(struct token *t);

// tree.c
struct ASTnode *mkastnode(int op, int type, struct ASTnode *left,
                          struct ASTnode *mid, struct ASTnode *right,
                          int intvalue);
struct ASTnode *mkastleaf(int op, int type, int intvalue);
struct ASTnode *mkastunary(int op, int type, struct ASTnode *left,
                           int intvalue);

// expr.c
struct ASTnode *binexpr(int ptp);
struct ASTnode *funccall();

// gen.c
int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genprimsize(int type);
int genlabel();

// cg.c
void freeall_registers();
void cgpreamble();
void cgfuncpreamble(int id);
void cgpostamble();
void cgfuncpostamble(int id);
int cgloadint(int value);
int cgloadglob(int id);
int cgstorglob(int reg, int id);
void cgglobsym(int id);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(int type);
int cgcall(int r, int id);
int cgreturn(int r, int id);
int cgaddress(int id);
int cgderef(int r, int type);

// misc.c
void match(int t, char *what);
void semi();
void ident();
void lbrace();
void rbrace();
void lparen();
void rparen();
void fatal(char *s);
void fatald(char *s, int d);
void fatals(char *s1, char *s2);
void fatalc(char *s, int c);

// decl.c
void var_declaration();
struct ASTnode *function_declaration();
struct ASTnode *assignment_statement();
struct ASTnode *print_statement();
struct ASTnode *if_statement();
struct ASTnode *compound_statement();

// sym.c
int findglob(char *s);
int addglob(char *name, int type, int stype, int endlabel);

// types.c
int type_compatible(int *left, int *right, int onlyright);
int pointer_to(int type);
int value_at(int type);
