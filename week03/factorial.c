#include <stdio.h>
#include <stdlib.h>

int factorial(int sum, int num) {
    if (num == 1) {
        return sum;
    } else {
        sum = sum * num;
        num --;
        return factorial(sum, num);
    }
}


int main(int n, char *args[]) {
    if (n >= 2) {
        int sum = 1;
        int num = atoi(args[1]);
        printf("%d\n",factorial(sum, num));
    } else printf("Please enter a number\n");
return 0;
}

