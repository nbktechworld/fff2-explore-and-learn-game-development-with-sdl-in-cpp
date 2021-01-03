#include "Game.h"

int main(int argc, char* argv[]) {
  Game game;

  game.initialize();
  game.createWindow();
  game.setPlayerColor();

  game.loadGameMap();
  game.printGameMap();
  game.setupGameMapSurface();

  game.setPlayerPosition();

  game.run();

  // SDL_Delay(20000);

  game.shutdown();

  return 0;
}
