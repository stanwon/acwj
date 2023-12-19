#include "1defs.h"
#include "data.h"
#include "decl.h"

int type_compatible(int *left, int *right, int onlyright) {
  if (P_VOID == *left || P_VOID == *right) {
    return 0;
  }

  if (*left == *right) {
    *left = *right = 0;
    return 1;
  }

  if (P_CHAR == *left && P_INT == *right) {
    *left = A_WIDEN;
    *right = 0;
    return 1;
  }

  if (P_INT == *left && P_CHAR == *right) {
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
