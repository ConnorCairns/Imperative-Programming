#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 5
#define ARRAY_LENGTH 6
#define MAX_DATA 63
#define DATA 192
#define DY 64
#define TOOL 128
#define COLOUR 3
#define TARGETX 4
#define TARGETY 5
#define DY 64
#define BLOCK 2

typedef struct command {
    int x, y, tx, ty, colour;
} command;

typedef struct commandArray {
    int n;
    struct command **array;
} commandArray;

command *newCommand(int tx, int ty, int colour, int count) {
    command *new;
    new = malloc(sizeof(command));
    new->tx = tx;
    new->ty = ty;
    new->colour = colour;
    new->x = tx - count;
    new->y = ty;
    return new;
}

commandArray *newCommandArray() {
    commandArray *new;
    new = malloc(sizeof(commandArray));
    new->array = malloc(sizeof(command));
    new->n = 0;
    return new;
}

void freeArray(commandArray *cArray) {
    for(int i = 0; i < cArray->n; i++) {
        free(cArray->array[i]);
    }
    free(cArray->array);
    free(cArray);
}

void addCommand(command *c, commandArray *a) {
    int size = sizeof(command) * (a->n + 1);
    a->array = realloc(a->array, size);
    a->array[a->n] = c;
    a->n++;
}

FILE *createFile(commandArray *a, char *filename) {
    strtok(filename, ".");
    strcat(filename, ".sk");
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot open %s\n",filename);
        exit(1);
    }
    return f;
}

commandArray *encode(int height, int width, unsigned char image[height][width]) {
    int count = 0;
    commandArray *cArray = newCommandArray();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((j+1) < width) {
                while ((j+2) < width & image[i][j] == image[i][j+1]) {
                    count++;
                    j++;
                }
                command *temp = newCommand(j, i, image[i][j], count);
                addCommand(temp, cArray);
                count = 0;
            }
        }
    }
    // printf("%d\n",cArray->n);
    // for(int i = 0; i < cArray->n; i++) {
        // printf("x:%d tx:%d y:%d ty:%d\n",cArray->array[i]->x,cArray->array[i]->tx, cArray->array[i]->y, cArray->array[i]->ty);
    // }
    return cArray;
}

void addToFile(FILE *f, commandArray *a) {
    fputc(TOOL, f);
    fputc(DATA, f);
    fputc(DATA, f);
    fputc(DATA, f);
    for(int i = 0; i < a->n; i++) {
        printf("%d: c:%d x:%d y:%d\n",i, a->array[i]->colour, a->array[i]->tx, a->array[i]->ty);
        int colour = 255;
        while (colour >= MAX_DATA) {
            fputc(MAX_DATA | DATA, f);
            colour -= MAX_DATA;
        }
        if (colour > 0) fputc(DATA | colour, f);
        fputc(TOOL | COLOUR, f);

        fputc(TOOL | 1, f);
        int tx = a->array[i]->tx;
        while (tx >= MAX_DATA) {
            fputc(MAX_DATA | DATA, f);
            tx -= MAX_DATA;
        }
        if (tx > 0) fputc(DATA | tx, f);
        fputc(TOOL | TARGETX, f);
        
        int ty = a->array[i]->ty;
        while (ty >= MAX_DATA) {
            fputc(MAX_DATA | DATA, f);
            ty -= MAX_DATA;
        }
        if (ty > 0) fputc(DATA | ty, f);
        fputc(TOOL | TARGETY, f);
        fputc(DY, f);

        fputc(TOOL, f);
        // int x = a->array[i]->tx;
        // while (x >= MAX_DATA) {
            // fputc(MAX_DATA | DATA, f);
            // x -= MAX_DATA;
        // }
        // if (x > 0) fputc(DATA | x, f);
        fputc(TOOL | TARGETX, f);
        
        int y = a->array[i]->ty + 1;
        while (y >= MAX_DATA) {
            fputc(MAX_DATA | DATA, f);
            y -= MAX_DATA;
        }
        if (y > 0) fputc(DATA | y, f);
        fputc(TOOL | TARGETY, f);
        fputc(DY, f);
    }
}

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

    unsigned char image[atoi(height)][atoi(width)];

    unsigned char b = fgetc(f);
    for (int i = 0; i < atoi(height); i++) {
        for (int j = 0; j < atoi(width); j++) {
            if (b == '\n') continue;
            image[i][j] = b;
            b = fgetc(f);
        }
    }
    commandArray *c = encode(atoi(height), atoi(width), image);
    FILE *skFile = createFile(c, filename);
    addToFile(skFile, c);
    // for (int i = 0; i < atoi(height); i++) {
        // for (int j = 0; j < atoi(width); j++) {
            // printf("%d ",image[i][j]);
        // }
    // }
    // printf("\n");
    freeArray(c);
    fclose(f);
    fclose(skFile);
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
