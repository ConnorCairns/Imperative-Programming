#include <stdio.h> 

int square(int num) {
    return (num*num);
}

int main(void) { 
    int num;
    printf("Please enter a number\n");
    scanf("%d", &num);
    printf("The square of %d is %d\n",num,square(num)); 
    return 0; 
}


