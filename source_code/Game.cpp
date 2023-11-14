#include "Game.h"
#include "SDL2/SDL_image.h"
#include "stdio.h"

Game::Game() : obstacle(1280, 500, 400, 400, 7), quit(false) {
}

Game::~Game() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL Initialization Error: %s\n", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        fprintf(stderr, "SDL_image Initialization Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    printf("Initialization Complete\n");

    win = SDL_CreateWindow("Sky Run", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, 0);
    if (!win) {
        fprintf(stderr, "SDL Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        fprintf(stderr, "SDL Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }

    backgroundTexture = loadTexture("images/Background.png", rend);
    playerTexture = loadTexture("images/Player.png", rend);
    obstacleTexture = loadTexture("images/Obstacle.png", rend);

    if (!backgroundTexture || !playerTexture || !obstacleTexture) {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }

    // Get the width and height of the window
    SDL_GetWindowSize(win, &winWidth, &winHeight);

    // Initialize variables for background position
    bgX = 0;
    bgSpeed = 5; // Adjust the speed of the scrolling
    resetThreshold = -winWidth; // Reset bgX when it goes beyond this threshold

    // Initialize variables for player position
    playerX = 50;
    playerY = winHeight / 2;
    playerWidth = 500;
    playerHeight = 500;
    playerSpeed = 30;

    return true;
}

void Game::run() {
    while (!quit) {
        handleEvents();
        update();
        render();
        SDL_Delay(10); // Adjust as needed for your target frame rate
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    playerY -= playerSpeed;
                    break;
                case SDLK_DOWN:
                    playerY += playerSpeed;
                    break;
            }
        }
    }
}

void Game::update() {
    obstacle.move();
    bgX -= bgSpeed;

    if (bgX <= resetThreshold) {
        bgX = 0;
    }
}

void Game::render() {
    SDL_RenderClear(rend);

    for (int i = 0; i < 2; ++i) {
        SDL_Rect bgRect = {bgX + i * winWidth, 0, winWidth, winHeight};
        SDL_RenderCopy(rend, backgroundTexture, NULL, &bgRect);
    }

    SDL_Rect playerRect = {playerX, playerY, playerWidth, playerHeight};
    SDL_RenderCopy(rend, playerTexture, NULL, &playerRect);

    SDL_Rect obstacleRect = {obstacle.x, obstacle.y, obstacle.width, obstacle.height};
    SDL_RenderCopy(rend, obstacleTexture, NULL, &obstacleRect);

    SDL_RenderPresent(rend);
}

SDL_Texture* Game::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        fprintf(stderr, "SDL Surface Creation Error: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        fprintf(stderr, "SDL Texture Creation Error: %s\n", SDL_GetError());
        return nullptr;
    }

    return texture;
}
