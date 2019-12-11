// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define OPCODE_SHIFT 6
#define MASK_SIGNIFICANT 63

// Allocate memory for a drawing state and initialise it
state *newState() {
  //TO DO
  state *new;
  new = malloc(sizeof(state));
  new->x = new->y = new->tx = new->ty = new->start = new->data = 0;
  new->end = false;
  new->tool = LINE;
  return new; // this is a placeholder only
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  return b >> OPCODE_SHIFT;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  int num = b & MASK_SIGNIFICANT;
  if ((b & 32) == 32) return (num - 64);
  else return num;
}

void updateLocation(state *s) {
  s->x = s->tx;
  s->y = s->ty;
}

void draw(display *d, state *s, int operand) {
  if (s->tool == LINE) line(d, s->x, s->y, s->tx, s->ty);
  else if (s->tool == BLOCK) {
    s->tx = s->tx - s->x;
    s->ty = s->ty - s->y;
    block(d, s->x, s->y, s->tx, s->ty);
  }
  s->data = 0;
  updateLocation(s);
}

void selectTool(display *d, state *s, int operand) {
  if (operand == COLOUR) colour(d, s->data);
  else if (operand == TARGETX) s->tx = s->data;
  else if (operand == TARGETY) s->ty = s->data;
  else if (operand == SHOW) show(d);
  else if (operand == PAUSE) pause(d, s->data);
  else if (operand == NEXTFRAME) {
    //s->start--;
    printf("%d\n",s->start);
    s->end = true;
  }
  else s->tool = operand;
  s->data = 0;
  //s->start++;
}

void data(state *s, int operand) {
  s->data = (s->data << 6) | (operand & MASK_SIGNIFICANT);
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  int opcode = getOpcode(op);
  int operand = getOperand(op);

  if (opcode == DX) {
    s->tx = s->tx + operand;
  }
  else if (opcode == DY) {
    s->ty = s->ty + operand;
    draw(d, s, operand);
  }
  else if (opcode == TOOL) {
    selectTool(d, s, operand);
  }
  else if (opcode == DATA) {
    data(s, operand);
  }
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {
  char *filename = getName(d);
  FILE *f = fopen(filename, "rb");
  if (f == NULL | data == NULL) return (pressedKey == 27);
  state *s = (state*) data;
  if (s->end) printf("hahayes\n");

  while (! feof(f) && ! s->end) {
    byte b = fgetc(f);
    s->start++;
    obey(d, s, b);
  }
  fclose(f);

    //TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
    //NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
    //NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
    //NOTE: TO GET THE FILENAME... char *filename = getName(d);
    //NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART
    //      THE 'START' FIELD AFTER CLOSING THE FILE
  show(d);
  s->x = s->y = s->tx = s->ty = s->data = 0;
  s->tool = LINE;
  s->end = false;
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
