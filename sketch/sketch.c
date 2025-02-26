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
  else if (operand == NEXTFRAME) s->end = true;
  else s->tool = operand;
  s->data = 0;
}

void data(state *s, int operand) {
  s->data = (s->data << OPCODE_SHIFT) | (operand & MASK_SIGNIFICANT);
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

  fseek(f, s->start, SEEK_SET);
  while (! feof(f) && ! s->end) {
    byte b = fgetc(f);
    obey(d, s, b);
  }

  if (s->end) s->start = ftell(f);
  if (feof(f)) s->start = 0;
  fclose(f);

  show(d);
  s->x = s->y = s->tx = s->ty = s->data = s->end = 0;
  s->tool = LINE;
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeDisplay(d);
  freeState(s);
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
