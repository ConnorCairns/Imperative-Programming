#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>

#define MAX 127
#define MIN -128

// Converts int to binary by shifting bits to the right by the count so only one bit remains, then doing bitwise AND with 1
void toBinary(int num, int bits, char *binary) {
    int limit = (bits - 2);
    for (int i = limit; i >= 0; i--) {
        int bit = (num >> i) & 1;
        sprintf(binary,"%s%d",binary,bit);
    }
}

// Prints the sign of an int by shifting all bits other than the sign bit to the right and doing bitwise AND with 1 on remaining bit
int sign(int num) {
    long shift = (sizeof(num)*8) - 1;
    int sign = (num >> shift & 1);
    return sign;
}

// If statement to print if it is negative or positive along with the sign bit
void signPrint(int sign) {
    if (sign == 1) printf("Sign: %d (Negative)\n",1);
    else printf("Sign: %d (Positive)\n",0);
}

// Check function which uses isalnum() to check if input is a digit or alphabetic character, if not return -1
// Then checks if an alphabetic character was entered and if more than one was entered return -1
// Or if the number is outside the bounds return -1
int charCheck(char type[], char input[]) {
    int num = atoi(input);
    int length = strlen(input);
    int i;
    ((input[0] == '-') && (length > 1)) ? (i = 1) : (i = 0); // Checking if number is negative, if so start for loop with i = 1 to miss "-"
    for ( ;i < length; i++) {
        if (isalnum(input[i]) == 0) {
            return -1;
        }
    }
    if ((isalpha(input[0]) != 0 && length > 1)) return -1;
    else if ((isdigit(input[1]) != 0 && num < MIN) || (isdigit(input[0]) != 0 && num > MAX)) return -1;
    return 1;
}

int intCheck(char type[], char input[]) {
    int length = strlen(input);
    int i;
    ((input[0] == '-') && (length > 1)) ? (i = 1) : (i = 0); // Checking if number is a negative number, if so start loop at 1 instead of 0
    for (; i < length; i++) {
        if (isdigit(input[i]) == 0) return -1; // If character is an alphabetic character, if so return -1
    }
    long num = atol(input); // Converting from string to long so number can be compared with INT_MIN and INT_MAX
    if ((num < INT_MIN) || (num > INT_MAX)) return -1;

    return 2;
}

int floatCheck(char type[], char input[]) {
    int length = strlen(input);
    int period = 0;
    int i;
    ((input[0] == '-') && (length > 1)) ? (i = 1) : (i = 0);
    for ( ; i < length; i++) { // Counting number of periods
        if (input[i] == '.') {
            period++;
        } else if ((isdigit(input[i]) == 0) && (input[i] != '.')) { // Checking all characters are digits, if not return -1
            return -1;
        }
    }
    if (period > 1) return -1; // More than one period (e.g 1.22.3) will return -1
    else return 3;
}

// Calls the correct function to parse user input
int check(char type[], char input[]) {
    if (strcmp(type, "char") == 0) return charCheck(type, input);
    else if (strcmp(type, "int") == 0) return intCheck(type, input);
    else if (strcmp(type, "float") == 0) return floatCheck(type, input);
    else return -1;
}

// First checks if char is an alphabetic character, if so print the ASCII value by using atoi() to convert to int
// Otherwise if input was a single digit, print ASCII value by adding 48
// Then sign() and toBinary() are called
void charConvert(char input[]) {
    int num = atoi(input);
    if (isalpha(input[0]) != 0) {
        for (int i = 0; i < strlen(input); i++) {
            num = input[i];
            printf("Char %s ASCII value: %d\n", input, num);
        }
    } else if (num >= 0 && num < 10) {
        printf("Char %s ASCII value: %d\n", input, num + 48);
    }
    int signResult = sign(num);
    signPrint(signResult);
    char binary[8] = "";
    toBinary(num, 8, binary);
    printf("Binary: %s\n",binary);
    printf("Hex: %X\n", num);
}

// Converts input to int using atoi() then calls sign() and toBinary()
void intConvert(char input[]) {
    int32_t num = atoi(input);
    int signResult = sign(num);
    signPrint(signResult);
    char binary[32] = "";
    toBinary(num, 32, binary);
    printf("Binary: %s\n",binary);
    printf("Hex: %X\n", num);
}

// It is possible to do bitwise operators on a float by using a pointer and casting the float to an int, then derefrencing when you want to use the operators
// Reference: https://web.archive.org/web/20160324044710/http://www.cs.umd.edu/class/sum2003/cmsc311/Notes/BitOp/bitshift.html
void floatConvert(char input[]) {
    float num = atof(input);
    int *p = (int *)&num;
    int signResult = sign(num);
    signPrint(signResult);
    char binary[32] = "";
    toBinary(*p, 32, binary);
    printf("Exponent: %.8s\n",binary); // Using precision specifier to only print first 8 bits
    char *mantissa = &binary[8]; // Creating pointer to start of mantissa
    printf("Mantissa: %s\n",mantissa);
}

void assert(int line, bool b) {
    if (b) return;
    printf("The test on line %d fails.\n", line);
    exit(1);
}

void testCharCheck() {
    assert(__LINE__, check("char", "1") == 1);
    assert(__LINE__, check("char", "-1") == 1);
    assert(__LINE__, check("char", "20") == 1);
    assert(__LINE__, check("char", "-20") == 1);
    assert(__LINE__, check("char", "--20") == -1);
    assert(__LINE__, check("char", "a") == 1);
    assert(__LINE__, check("char", "ab") == -1);
    assert(__LINE__, check("char", "!") == -1);
    assert(__LINE__, check("char", "-") == -1);
}

void testIntCheck() {
    assert(__LINE__, check("int", "1") == 2);
    assert(__LINE__, check("int", "12") == 2);
    assert(__LINE__, check("int", "-1") == 2);
    assert(__LINE__, check("int", "--1") == -1);
    assert(__LINE__, check("int", "a") == -1);
    assert(__LINE__, check("int", "?") == -1);
    assert(__LINE__, check("int", "2147483648") == -1);
    assert(__LINE__, check("int", "-2147483649") == -1);
}

void testFloatCheck() {
    assert(__LINE__, check("float", "1") == 3);
    assert(__LINE__, check("float", "1.2") == 3);
    assert(__LINE__, check("float", "-1.2") == 3);
    assert(__LINE__, check("float", "--1.2") == -1);
    assert(__LINE__, check("float", "a") == -1);
    assert(__LINE__, check("float", "1..2") == -1);
}

void testSign() {
    assert(__LINE__, sign(10) == 0);
    assert(__LINE__, sign(-10) == 1);
    assert(__LINE__, sign('a') == 0);
}

void test(){
    testCharCheck();
    testIntCheck();
    testSign();
    testFloatCheck();
    printf("All tests passed\n");
}

int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) {
        test();
    } else if (n == 3) {
        int result = check(args[1], args[2]);
        if (result == -1) printf("Error with inputs\n");
        else if (result == 1) charConvert(args[2]);
        else if (result == 2) intConvert(args[2]);
        else if (result == 3) floatConvert(args[2]);
    } else {
        fprintf(stderr, "Use e.g.: ./visualise [type] [input]\n");
        exit(1);
    }
    return 0;
}
