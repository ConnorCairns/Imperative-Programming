/* Find the limits of the int type */
#include <stdio.h>
#include <limits.h>

int largestInt(int min, long max) {
  long mid = (min + max) / 2;
  int test = mid;
  if (test == mid) {
    min = mid;
  } else {
    max = mid;
  }
  if (max - min < 2) {
    return test;
  }

  return largestInt(min, max);

}

int main() {
  int n = largestInt(0,2500000000);
  printf("The largest int is %d\n", n);
  printf("The official largest int is %d\n", INT_MAX);
  return 0;
}
