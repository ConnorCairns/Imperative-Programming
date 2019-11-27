#include <string.h>
#include <stdio.h>

int plusOne(int x) {
  return x + 1;
}

int sumArray(int n, int array[n]) {
  int sum = 0;
  for(int i = 0; i < n; i++) {
    sum += array[i];
  }
  return sum;
}

int main(void) {
  int x = 7;
  // int array[3] = {1,2,3};
  printf("Hello World!\n");
  printf("%d + 1 = %d\n",x,plusOne(x));

  int a = 1;
  int * pointer_to_a = &a;
  printf("The value of a is %d\n",a);
  printf("The value of a also is %d\n", *pointer_to_a);

  return 0;
}
