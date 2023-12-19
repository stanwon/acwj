int scan(struct token *t);
struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *mid,
                          struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);

struct ASTnode *binexpr(int ptp);

int interpretAST(struct ASTnode *n);

int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);

void freeall_registers();
void cgpreamble();
void cgfuncpreamble(char *name);
void cgfuncpostamble();
int cgloadint(int value);
int cgloadglob(char *identifier);
int cgstorglob(int reg, char *name);
void cgglobsym(char *sym);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglessthan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);

void statements();

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

void var_declaration();
struct ASTnode *function_declaration();
struct ASTnode *assignment_statement();
struct ASTnode *print_statement();
struct ASTnode *if_statement();
struct ASTnode *compound_statement();

int findglob(char *s);
int addglob(char *name);
