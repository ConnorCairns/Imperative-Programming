#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 5
#define ARRAY_LENGTH 6

void pgmToSk(char *filename) {
    strcat(filename, ".pgm");
    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(1);
    }

    fseek(f, 3, SEEK_SET);

    char width[ARRAY_LENGTH];
    char height[ARRAY_LENGTH];
    char maxval[ARRAY_LENGTH];
    fgets(width, LENGTH, f);
    fgets(height, LENGTH, f);
    fgets(maxval, LENGTH, f);
    printf("%s\n",width);
    printf("%s\n",height);
    printf("%s\n",maxval);

    // char haha[100];
    // fgets(haha, 100, f);
    // printf("haha: %s\n",haha);
    // fgets(haha, 100, f);
    // printf("haha 2: %s\n",haha);
    // fgets(haha, 100, f);
    // printf("haha 3: %s\n",haha);
    char haha[100];
    fgets(haha, 99, f);
    while (!feof(f)) {
        printf("%s\n",haha);
        fgets(haha, 99, f);
        
    }
    fclose(f);
}

void process(char *filename) {
    char *str = filename;
    char *token = strtok(str, ".");
    token = strtok(NULL, ".");
    
    if (strcmp(token, "pgm") == 0) pgmToSk(filename);
}

int main(int n, char *args[n]) {
    if (n == 2) {
        process(args[1]);
    } else {
        printf("Usage: ./converter [filename]\n");
        exit(1);
    }
    return 0;
}
