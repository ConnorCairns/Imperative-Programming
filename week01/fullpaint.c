/* Find the area of paint I need. */
#include <stdio.h>

// Calculate area of walls and ceiling.
int area(int length, int width, int height) {
  int sides = 2 * length * height;
  int ends = 2 * width * height;
  int ceiling = length * width;
  return sides + ends + ceiling;
}

// Read room size and print area of paint.
int main(void) {
  int lengths, width, height;
  printf("Enter room length:\n");
  scanf("%d", &lengths);
  printf("Enter room width:\n");
  scanf("%d", &width);
  printf("Enter room height:\n");
  scanf("%d", &height);
  int total = area(lengths, width, height);
  printf("The paint area is: %d\n", total);
  return 0;
}
