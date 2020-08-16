#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
  std::cout << "Starting" << std::endl;
  int WINDOW_WIDTH = 640;
  int WINDOW_HEIGHT = 480;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Hello World",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0
  );
  if (window == NULL) {
    std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    return 2;
  }

  SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
  // SDL_Surface* surface = SDL_CreateRGBSurface(
  //   0,
  //   640,
  //   480,
  //   32,
  //   0,
  //   0,
  //   0,
  //   0
  // );


  SDL_Event event;

  bool quit = false;
  SDL_Rect playerPosition = {0, 0, 32, 32};

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RIGHT) {
          // update player position
          if (playerPosition.x + 32 < WINDOW_WIDTH) {
            playerPosition.x = playerPosition.x + 32;
          }
        }
        else if (event.key.keysym.sym == SDLK_DOWN) {
          if (playerPosition.y + 32 < WINDOW_HEIGHT) {
            playerPosition.y = playerPosition.y + 32;
          }
        }
        else if (event.key.keysym.sym == SDLK_LEFT) {
          if (playerPosition.x - 32 >= 0) {
            playerPosition.x = playerPosition.x - 32;
          }
        }
        else if (event.key.keysym.sym == SDLK_UP) {
          if (playerPosition.y - 32 >= 0) {
            playerPosition.y = playerPosition.y - 32;
          }
        }
      }
    }

    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 255));
    SDL_Rect rectangle1;
    rectangle1.w = WINDOW_WIDTH * 3 / 4;
    rectangle1.h = WINDOW_HEIGHT * 3 / 4 ;
    rectangle1.x = (WINDOW_WIDTH - rectangle1.w) / 2;
    rectangle1.y = (WINDOW_HEIGHT - rectangle1.h) / 2;
    SDL_FillRect(windowSurface, &rectangle1, SDL_MapRGB(windowSurface->format, 255, 0, 0));

    SDL_Rect rectangle2;
    rectangle2.w = WINDOW_WIDTH / 2;
    rectangle2.h = WINDOW_HEIGHT / 2;
    rectangle2.x = (WINDOW_WIDTH - rectangle2.w) / 2;
    rectangle2.y = (WINDOW_HEIGHT - rectangle2.h) / 2;
    SDL_FillRect(windowSurface, &rectangle2, SDL_MapRGB(windowSurface->format, 0, 255, 0));

    Uint32 playerColor = SDL_MapRGB(windowSurface->format, 128, 128, 128);
    SDL_FillRect(windowSurface, &playerPosition, playerColor);

    SDL_UpdateWindowSurface(window);
  }



  // SDL_Delay(20000);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
