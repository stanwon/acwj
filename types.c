#include "0defs.h"
#include "1decl.h"
#include "2data.h"

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
