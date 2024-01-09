#include "0defs.h"
#include "1decl.h"
#include "2data.h"

int inttype(int type) {
  if (P_CHAR == type || P_INT == type || P_LONG == type) {
    return 1;
  }
  return 0;
}

int ptrtype(int type) {
  if (P_CHARPTR == type || P_INTPTR == type || P_LONGPTR == type ||
      P_VOIDPTR == type) {
    return 1;
  }
  return 0;
}

int pointer_to(int type) {
  switch (type) {
  case P_VOID:
    return P_VOIDPTR;
  case P_CHAR:
    return P_CHARPTR;
  case P_INT:
    return P_INTPTR;
  case P_LONG:
    return P_LONGPTR;
  default:
    fatald("Unrecognised in pointer_to: type", type);
  }
}

int value_at(int type) {
  switch (type) {
  case P_VOIDPTR:
    return P_VOID;
  case P_CHARPTR:
    return P_CHAR;
  case P_INTPTR:
    return P_INT;
  case P_LONGPTR:
    return P_LONG;
  default:
    fatald("Unrecognised in value_at: type", type);
  }
}

struct ASTnode *modify_type(struct ASTnode *tree, int rtype, int op) {
  int ltype;
  int lsize, rsize;

  ltype = tree->type;

  if (inttype(ltype) && inttype(rtype)) {
    if (ltype == rtype) {
      return tree;
    }

    lsize = genprimsize(ltype);
    rsize = genprimsize(rtype);

    if (lsize > rsize) {
      return NULL;
    }

    if (rsize > lsize) {
      return mkastunary(A_WIDEN, rtype, tree, 0);
    }
  }

  if (ptrtype(ltype)) {
    if (0 == op && ltype == rtype) {
      return tree;
    }
  }

  if (A_ADD == op || A_SUBTRACT == op) {
    if (inttype(ltype) && ptrtype(rtype)) {
      rsize = genprimsize(value_at(rtype));
      if (rsize > 1) {
        return mkastunary(A_SCALE, rtype, tree, rsize);
      } else {
        return tree;
      }
    }
  }

  return NULL;
}

int type_compatible(int *left, int *right, int onlyright) {
  int leftsize, rightsize;

  if (*left == *right) {
    *left = *right = 0;
    return 1;
  }

  leftsize = genprimsize(*left);
  rightsize = genprimsize(*right);

  if (0 == leftsize || 0 == rightsize) {
    return 0;
  }

  if (leftsize < rightsize) {
    *left = A_WIDEN;
    *right = 0;
    return 1;
  }

  if (leftsize > rightsize) {
    if (onlyright) {
      return 0;
    }
    *left = 0;
    *right = A_WIDEN;
    return 1;
  }

  *left = *right = 0;
  return 1;
}
