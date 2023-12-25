#include "0defs.h"
#include "1decl.h"
#include "2data.h"

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
