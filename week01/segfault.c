/* Generate a segfault. */
#include <stdio.h>

int main() {
  int seq[3] = {10, 5, 4};
  int outOfBoundsIndex = 1000000000;   // index larger than 2 !
  printf("%d\n", seq[outOfBoundsIndex]);  // access outside array bounds
  return 0;
}
