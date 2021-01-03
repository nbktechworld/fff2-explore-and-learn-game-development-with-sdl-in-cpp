#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

class Game {
  int WINDOW_WIDTH = 640; // 20 tiles
  int WINDOW_HEIGHT = 480; // 15 tiles
  int TILE_SIZE = 32;
  SDL_Window* window;
  SDL_Surface* windowSurface;
  int tiles[300] = { 0 };
  SDL_Rect entrancePosition;
  SDL_Rect exitPosition;
  SDL_Surface* gameMapSurface;
  SDL_Rect playerPosition;
  SDL_Event event;
  Uint32 playerColor;
public:
  Game();
  void initialize();
  void shutdown();
  void createWindow();
  void loadGameMap();
  void printGameMap();
  SDL_Surface* createGameMapSurface();
  void setupGameMapSurface();
  void paintEntrance(SDL_Surface* surface);
  void paintExit(SDL_Surface* surface);
  void run();
  void handlePlayerInput();
  void setPlayerPosition();
  void setPlayerColor();
  int mapCoordinateToTileId(SDL_Rect position);
  bool isWalkableTile(int tileId);
};

#endif
