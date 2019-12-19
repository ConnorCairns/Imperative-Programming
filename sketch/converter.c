#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LENGTH 5
#define ARRAY_LENGTH 6
#define MAX_DATA 63
#define DATA 192
#define DY 64
#define TOOL 128
#define COLOUR 3
#define TARGETX 4
#define TARGETY 5
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

//Uses run length encoding on x axis to encode image array
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
    return cArray;
}

//Calls DATA 6 times to fill up 32bit data value
void addData(FILE *f, int val) {
    uint8_t temp = 0;
    int count = 0;
    for (int i = 5; i >= 0; i--) {
        //Shifts bits to split it into 6 different DATA calls and masks result with MAX_DATA to get only 6 bits
        temp = (val >> 6*i) & MAX_DATA;
        //Count is used as a flag to see if there has been any data other than 0
        if (temp != 0) count++;
        //Data will only be added when it starts to have a non zero value, stopping uneccesary data calls
        if (count > 0) fputc(DATA | temp, f);
    }
}

void addToFile(FILE *f, commandArray *a, int width) {
    for(int i = 0; i < a->n; i++) {
        //Scaling colour to RGBA
        unsigned int c = a->array[i]->colour;
        //For greyscale RGB values all need to be the same so c is shifted by 8, 16 and 24 so colour has the same 8 bits of binary 3 times
        //A 1 is added at the end for A to make the image be at 0% opacity
        unsigned int colour = (c << 8) + (c << 16) + (c << 24) + 1;
        //Using DATA to add colour value
        if (colour >= MAX_DATA) {
            addData(f, colour);
        } else fputc(DATA | colour, f);
        fputc(TOOL | COLOUR, f);

        //Changing tool to LINE
        fputc(TOOL | 1, f);

        //Using DATA to add TARGETX value
        int tx = a->array[i]->tx;
        int tempx = tx;
        if (tx >= MAX_DATA) {
            addData(f, tx);
        } else fputc(DATA | tx, f);
        fputc(TOOL | TARGETX, f);

        //Using DATA to add TARGETY value
        int ty = a->array[i]->ty;
        int tempy = a->array[i]->ty + 1;
        if (ty >= MAX_DATA) {
            addData(f, ty);
        } else fputc(DATA | ty, f);
        fputc(TOOL | TARGETY, f);

        //Calling DY to draw line
        fputc(DY, f);

        //If reached the end of the line, reset x and increment y value
        if(tempx == width-2) {
            fputc(TOOL, f);
            addData(f, 0);
            fputc(TOOL | TARGETX, f);   
            //addData(f, tempy);
            if (tempy >= MAX_DATA) {
               addData(f, tempy);
            } else fputc(DATA | tempy, f);
            fputc(TOOL | TARGETY, f);
            fputc(DY, f);
        }
    }
}

void pgmToSk(char *filename) {
    strcat(filename, ".pgm");
    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(1);
    }

    //Checking file has P5 at start to make sure it it the correct format
    char format[ARRAY_LENGTH];
    fgets(format, 4, f);
    if (!(strcmp(format, "P5 ") == 0)) {
        fprintf(stderr, "File not of type P5\n");
        exit(1);
    }
    //Getting width, height and max greyval
    char width[ARRAY_LENGTH];
    char height[ARRAY_LENGTH];
    char maxval[ARRAY_LENGTH];
    fgets(width, LENGTH, f);
    fgets(height, LENGTH, f);
    fgets(maxval, LENGTH, f);

    unsigned char image[atoi(height)][atoi(width)];

    //Getting each byte from the file and adding it to image array
    unsigned char b = fgetc(f);
    for (int i = 0; i < atoi(height); i++) {
        for (int j = 0; j < atoi(width); j++) {
            image[i][j] = b;
            b = fgetc(f);
        }
    }

    commandArray *c = encode(atoi(height), atoi(width), image);
    FILE *skFile = createFile(c, filename);
    addToFile(skFile, c, atoi(width));
    freeArray(c);
    fclose(f);
    fclose(skFile);
    printf("Written to %s\n",filename);
}

//Check file has extension .pgm
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
