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
void dumpAST(struct ASTnode *n, int label, int level);

// expr.c
struct ASTnode *binexpr(int ptp);
struct ASTnode *funccall();
struct ASTnode *array_access();

// gen.c
int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genprimsize(int type);
int genlabel();
int genglobstr(char *str);

// cg.c
void freeall_registers();
void cgpreamble();
void cgfuncpreamble(int id);
void cgpostamble();
void cgfuncpostamble(int id);
int cgloadint(int value, int type);
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
int cgshlconst(int r, int val);
int cgstorderef(int r1, int r2, int type);
void cgglobstr(int l, char *str);
int cgloadglobstr(int id);

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
void var_declaration(int type);
int parse_type();
void global_declarations();
struct ASTnode *function_declaration(int type);
struct ASTnode *if_statement();
struct ASTnode *compound_statement();

// sym.c
int findglob(char *s);
int addglob(char *name, int type, int stype, int endlabel, int size);

// types.c
struct ASTnode *modify_type(struct ASTnode *tree, int rtype, int op);
int inttype(int type);
int pointer_to(int type);
int value_at(int type);
