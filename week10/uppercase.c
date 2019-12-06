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
    FILE *src = fopenCheck(args[1], "r");
    FILE *dst = fopenCheck(args[2], "w");

    const int max = 100;
    char line[max];
    fgets(line, max, src);
    while (! feof(src)) {
        int i = 0;
        while (line[i]) {
            line[i] = (line[i] - 32);
            i++;
        }
        fprintf(dst, "%s",line);
        fgets(line, max, src);
    }

    fclose(src);
    fclose(dst);
}
