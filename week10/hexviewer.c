#include <stdio.h>
#include <stdlib.h>

FILE *fopenCheck(char *file, char *mode) {
    FILE *p = fopen(file, mode);
    if (p != NULL) return p;
    fprintf(stderr, "Cant open %s: ", file);
    fflush(stderr);
    perror("");
    exit(1);
}

int main(int n, char *args[n]) {
    FILE *src = fopenCheck(args[1], "rb");
    unsigned char b = fgetc(src);
    
    while (! feof(src)) {
        printf("%x ",b);
        b = fgetc(src);
        if (b == '\n') printf("\n");
    }

    fclose(src);
}
