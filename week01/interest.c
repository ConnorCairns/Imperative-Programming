#include <stdio.h>

double add(double money, double interest) {
    double final_amount = money + (money * (interest / 100));
    return (final_amount);
}

int main(void) {
    double a, i;
    printf("Please enter the investment amount\n");
    scanf("%lf", &a);
    printf("Please enter interest rate\n");
    scanf("%lf", &i);
    printf("Adding %lf%% interest to %lf gives %lf\n", i, a, add(a, i));
    return 0;
}
