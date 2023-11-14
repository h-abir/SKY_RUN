#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"  // Include SDL_image for IMG_Load
#include "Obstacle.h"

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();

private:
    SDL_Window* win;
    SDL_Renderer* rend;
    SDL_Texture* backgroundTexture;
    SDL_Texture* playerTexture;
    SDL_Texture* obstacleTexture;
    Obstacle obstacle;

    int winWidth;
    int winHeight;
    int bgX;
    int bgSpeed;
    int resetThreshold;
    int playerX;
    int playerY;
    int playerWidth;
    int playerHeight;
    int playerSpeed;

    bool quit;

    void handleEvents();
    void update();
    void render();

    // Declare the loadTexture function
    SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
};

#endif // GAME_H
