int scan(struct token *t);
void reject_token(struct token *t);
struct ASTnode *mkastnode(int op, int type, struct ASTnode *left,
                          struct ASTnode *mid, struct ASTnode *right,
                          int intvalue);
struct ASTnode *mkastleaf(int op, int type, int intvalue);
struct ASTnode *mkastunary(int op, int type, struct ASTnode *left,
                           int intvalue);

struct ASTnode *binexpr(int ptp);
struct ASTnode *funccall();

int interpretAST(struct ASTnode *n);

int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genprimsize(int type);
int genlabel();

void freeall_registers();
void cgpreamble();
void cgfuncpreamble(int id);
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
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(int type);
int cgcall(int r, int id);
int cgreturn(int r, int id);

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
int addglob(char *name, int type, int stype, int endlabel);

int type_compatible(int *left, int *right, int onlyright);
