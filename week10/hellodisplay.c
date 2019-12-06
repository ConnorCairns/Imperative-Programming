#include "display.h"
#include <stdio.h>
#include <stdbool.h>

#define SCALE 32
typedef struct location {int x, y;} location;

// Our function to be called repeatedly by the display module
bool functionToBeCalledRepeatedly(display *d, void *data, const char pressedKey) {
    location *l = (location*) data;

    colour(d, 0xFF); //set colour to black to clear old box
    block(d, l->x, l->y, SCALE, SCALE); 

    switch (pressedKey) {
        case 79: l->x += SCALE; break; //right
        case 80: l->x -= SCALE; break; //left
        case 81: l->y += SCALE; break; //down
        case 82: l->y -= SCALE; break; //up
    }

     // limit position state to size of the display window (wrap-around)
    *l = (location) { (l->x + getWidth(d)) % getWidth(d), (l->y + getHeight(d)) % getHeight(d)};

    colour(d, 0xFFFF);
    block(d, l->x, l->y, SCALE, SCALE);
    show(d);

    return (pressedKey == 27);
}

int main() {
    display *d = newDisplay("Hello Display", 1280, 720);
    location *l = malloc(sizeof(location));
    *l = (location) {0, 0};
    run(d, l, functionToBeCalledRepeatedly);
    
    free(l);
    freeDisplay(d);
}
