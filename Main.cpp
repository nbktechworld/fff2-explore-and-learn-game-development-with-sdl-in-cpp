#include <fstream>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

enum TileType {
  Floor, // 0
  Wall   // 1
};

int WINDOW_WIDTH = 640; // 20 tiles
int WINDOW_HEIGHT = 480; // 15 tiles
int TILE_SIZE = 32;

SDL_Surface* createGameMapSurface(int* tiles) {
  SDL_Surface* surface = SDL_CreateRGBSurface(
    0,
    640,
    480,
    32,
    0,
    0,
    0,
    0
  );

  int row = 0;
  int column = 0;

  SDL_Rect rectangle;
  rectangle.w = 32;
  rectangle.h = 32;

  while (row < 15) {// 480 / 32
    while (column < 20) {
      rectangle.x = column * 32; //todo: TILE_SIZE CONSTANT
      rectangle.y = row * 32;
      int currentTileId = tiles[row * 20 + column];
      Uint32 tileColor;
      if (currentTileId == 0) {
        // walkable tile
        tileColor = SDL_MapRGB(surface->format, 0, 0, 0);
      }
      else if (currentTileId == 1) {
        // wall tile (blocks you)
        tileColor = SDL_MapRGB(surface->format, 255, 255, 255);
      }

      SDL_FillRect(surface, &rectangle, tileColor);

      column++;
    }

    column = 0;
    row++;
  }

  return surface;
}

void printGameMap(int* tiles, SDL_Rect entrancePosition, SDL_Rect exitPosition) {
  std::cout << "Entrance: (" << entrancePosition.x << ", " << entrancePosition.y << ")" << std::endl;
  std::cout << "Exit: (" << exitPosition.x << ", " << exitPosition.y << ")" << std::endl;

  for (int i = 0; i < 300; i++) {
    if (i != 0 && i % 20 == 0) {
      std::cout << std::endl;
    }
    std::cout << tiles[i] << " ";
  }
  std::cout << std::endl;
}

void loadGameMap(int* tiles, SDL_Rect &entrancePosition, SDL_Rect &exitPosition) {
  std::ifstream gameMapFile("GameMap.txt");

  if (gameMapFile.is_open()) {
    int entranceLine;
    int entranceColumn;
    gameMapFile >> entranceLine;
    gameMapFile >> entranceColumn;
    entrancePosition.x = entranceColumn * TILE_SIZE;
    entrancePosition.y = entranceLine * TILE_SIZE;

    int exitLine, exitColumn;
    gameMapFile >> exitLine >> exitColumn;
    exitPosition.x = exitColumn * TILE_SIZE;
    exitPosition.y = exitLine * TILE_SIZE;

    int tileId;
    int index = 0;
    while (gameMapFile >> tileId) {
      tiles[index] = tileId;
      index++;
    }

    gameMapFile.close();
  }
}

// Function to map the coordinates with the element in array of tiles
int mapCoordinateToTileId(SDL_Rect position, int tiles[300]) {
  int tileSize = 32;
  int tilesAcross = 20;
  int index = position.y / tileSize * tilesAcross + position.x / tileSize;

  return tiles[index];
}

bool isWalkableTile(int tileId) {
  if (tileId == Floor) {
    return true;
  }

  return false;
}

void handlePlayerInput(SDL_Event event, SDL_Rect& playerPosition, int gameMapTiles[300]) {
  SDL_Rect newPosition = playerPosition;
  int nextTileId;

  if (event.key.keysym.sym == SDLK_RIGHT) {
    // update player position
    if (playerPosition.x + 32 < WINDOW_WIDTH) {
      // check if hitting a wall
      newPosition.x = newPosition.x + 32;
      nextTileId = mapCoordinateToTileId(newPosition, gameMapTiles);
      if (isWalkableTile(nextTileId)) {
        playerPosition.x = playerPosition.x + 32;
      }
    }
  }
  else if (event.key.keysym.sym == SDLK_DOWN) {
    if (playerPosition.y + 32 < WINDOW_HEIGHT) {
      newPosition.y = newPosition.y + 32;
      nextTileId = mapCoordinateToTileId(newPosition, gameMapTiles);
      if (isWalkableTile(nextTileId)) {
        playerPosition.y = playerPosition.y + 32;
      }
    }
  }
  else if (event.key.keysym.sym == SDLK_LEFT) {
    if (playerPosition.x - 32 >= 0) {
      newPosition.x = newPosition.x - 32;
      nextTileId = mapCoordinateToTileId(newPosition, gameMapTiles);
      if (isWalkableTile(nextTileId)) {
        playerPosition.x = playerPosition.x - 32;
      }
    }
  }
  else if (event.key.keysym.sym == SDLK_UP) {
    if (playerPosition.y - 32 >= 0) {
      newPosition.y = newPosition.y - 32;
      nextTileId = mapCoordinateToTileId(newPosition, gameMapTiles);
      if (isWalkableTile(nextTileId)) {
        playerPosition.y = playerPosition.y - 32;
      }
    }
  }
}

void paintEntrance(SDL_Surface* surface, SDL_Rect entrancePosition) {
  Uint32 color = SDL_MapRGB(surface->format, 128, 255, 128);
  SDL_FillRect(surface, &entrancePosition, color);
}

void paintExit(SDL_Surface* surface, SDL_Rect exitPosition) {
  Uint32 color = SDL_MapRGB(surface->format, 255, 128, 128);
  SDL_FillRect(surface, &exitPosition, color);
}

int main(int argc, char* argv[]) {
  std::cout << "Starting" << std::endl;

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

  int gameMapTiles[300] = { 0 };
  SDL_Rect entrancePosition = { 0, 0, TILE_SIZE, TILE_SIZE };
  SDL_Rect exitPosition = { 0, 0, TILE_SIZE, TILE_SIZE };
  loadGameMap(gameMapTiles, entrancePosition, exitPosition);
  printGameMap(gameMapTiles, entrancePosition, exitPosition);
  SDL_Surface* gameMapSurface = createGameMapSurface(gameMapTiles);
  if (gameMapSurface == NULL) {
    SDL_Log("Failed to create game map surface: %s", SDL_GetError());
    return 3;
  }
  paintEntrance(gameMapSurface, entrancePosition);
  paintExit(gameMapSurface, exitPosition);

  SDL_Event event;

  bool quit = false;
  SDL_Rect playerPosition = {entrancePosition.x, entrancePosition.y, 32, 32};

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      else if (event.type == SDL_KEYDOWN) {
        handlePlayerInput(event, playerPosition, gameMapTiles);
      }
    }

    if (SDL_BlitSurface(gameMapSurface, NULL, windowSurface, NULL) != 0) {
      SDL_Log("There was a problem blitting the game map surface onto the window surface: %s", SDL_GetError());
      return 4;
    };

    Uint32 playerColor = SDL_MapRGB(windowSurface->format, 128, 128, 128);
    SDL_FillRect(windowSurface, &playerPosition, playerColor);

    SDL_UpdateWindowSurface(window);
  }



  // SDL_Delay(20000);

  SDL_FreeSurface(gameMapSurface);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
