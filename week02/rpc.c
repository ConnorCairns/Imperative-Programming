#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <ctype.h>
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
// rock = 0
// scissor = 2
// paper = 1

void error() {
    printf("No or too many argument(s) given\n");
}

void strToLower(int n, char choice[]) {
    for (int i = 0; i < n; i++) {
        choice[i] = tolower(choice[i]);
    }
}

//convertToNum string input to int for comparisons, -1 = error
int convertToNum(const char choice[]) {
    //char lowerChoice[] = "";
    if (strcmp(&choice[0], "rock") == 0) { // & denotes an address in memory
        return 0;
    } else if (strcmp(&choice[0], "paper") == 0) {
        return 1;
    } else if (strcmp(&choice[0], "scissors") == 0) {
        return 2;
    } else return -1;
}

char *convertToStr(int num) { // * creates a pointer
    if (num == 0) return "rock";
    else if (num == 1) return "paper";
    else if (num == 2) return "scissors";
    else return "Something went wrong";
}

int win(int n) {
    if (n == 0) return 1;
    else if (n == 1) return 2;
    else if (n == 2) return 0;
    else return -1;
}

void rpc(char playerChoice[], int playerChoiceNum, int win) {
    srand(time(0));
    int cpu = rand() % 3;
    char *cpuChoice = convertToStr(cpu);
    printf("CPU choice: %s\n",cpuChoice);
    printf("Player choice: %s\n",playerChoice);
    
    if (playerChoiceNum == -1) printf("Bad input\n");
    else if (cpu == playerChoiceNum) printf("Draw!\n");
    else if (cpu == win) printf("CPU wins!\n");
    else printf("Player wins!\n");
}

int main(int n, char *args[n]) {
    if (n == 1 || n > 2) {
        error();
    } else {
        strToLower(NELEMS(args[1]), args[1]);
        int playerChoiceNum = convertToNum(args[1]);
        int beat = win(playerChoiceNum);
        rpc(args[1], playerChoiceNum, beat);
    }
    return 0;
}
