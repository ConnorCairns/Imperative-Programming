#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 5
#define ARRAY_LENGTH 6

typedef struct command {
    int x, y, tx, ty, colour;
} command;

typedef struct commandArray {
    int n;
    command **array;
} commandArray;

command *newCommand(int tx, int ty, int colour, int count) {
    command *new;
    new = malloc(sizeof(command));
    new->tx = tx;
    new->ty = ty;
    new->colour = colour;
    new->x = tx - count;
    new->y = ty - count;
    return new;
}

commandArray *newCommandArray() {
    commandArray *new;
    new = malloc(sizeof(commandArray));
    new->array = malloc(sizeof(command));
    new->n = 1;
    return new;
}

void freeArray(commandArray *commandArray) {
    for(int i = 0; i < commandArray->n; i++) {
        free(&commandArray->array[i]);
    }
    free(commandArray);
}

void addCommand(command *c, commandArray *a) {
    int size = sizeof(command) * (a->n + 1);
    a = realloc(a, size);
    //printf("%d\n",a->n);
    a->array[a->n] = c;
    a->n++;

}

void encode(int height, int width, unsigned char image[height][width]) {
    int colour, count = 1;
    commandArray *cArray = newCommandArray();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((j+1) < width) {
                if (j == width-1) {
                    command *temp = newCommand((j-1), i, colour, count);
                    addCommand(temp, cArray);
                    printf("%d\n",j);
                    printf("hahayes\n");
                    count = 1;
                    // ty = i;
                    // tx = j - 1;
                } else if (image[i][j] == image[i][j+1]) {
                    colour = image[i][j];
                    count++;
                } else {
                    printf("haha no\n");
                    command *temp = newCommand((j-1), i, colour, count);
                    addCommand(temp, cArray);
                    count = 1;
                }
            }
        }
        //printf("%d: %d count: %d\n",i,colour, count);
        count = 1;
    }
    // for (int i = 0; i < height; i++) {
        // for (int j = 0; j < width; j++) {
            
        // }
    // }

    printf("%d\n",cArray->n);
    for(int i = 0; i < cArray->n; i++) {
        printf("col:%d x:%d y:%d\n",cArray->array[i]->colour,cArray->array[i]->x,cArray->array[i]->y);
    }
    freeArray(cArray);
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
    encode(atoi(height), atoi(width), image);
    // for (int i = 0; i < atoi(height); i++) {
        // for (int j = 0; j < atoi(width); j++) {
            // printf("%d ",image[i][j]);
        // }
    // }
    // printf("\n");

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
    //freeArray();
    return 0;
}
