#include <SDL2/SDL.h>

int main (int argc, char** argv)
{
  SDL_Window* window = NULL;
  window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED,
           SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 90, 90, 255, 255 );
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  int quit = 0;
	SDL_Event e;
	while (!quit) {
    while (SDL_PollEvent(&e)) {
	    if (e.type == SDL_QUIT) quit = 1;
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
