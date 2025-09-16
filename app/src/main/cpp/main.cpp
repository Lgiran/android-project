#include "SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

struct Vector2 {
    float x, y;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
};

// بقیه کدهای شما...
struct Rocket {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool active;
    int type;
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    
    SDL_Texture* rocketTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* explosionTexture;
    
    Mix_Chunk* rocketSound;
    Mix_Chunk* explosionSound;
    Mix_Music* backgroundMusic;
    
    TTF_Font* font;
    
    std::vector<Rocket> rockets;
    int score;
    bool gameOver;
    
public:
    Game() : window(nullptr), renderer(nullptr), isRunning(true), 
             score(0), gameOver(false) {}
    
    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
            std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
            return false;
        }
        
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
            return false;
        }
        
        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
            return false;
        }
        
        window = SDL_CreateWindow("Rocket Game", 
                                 SDL_WINDOWPOS_CENTERED, 
                                 SDL_WINDOWPOS_CENTERED,
                                 SCREEN_WIDTH, SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Window could not be created! Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        renderer = SDL_CreateRenderer(window, -1, 
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            std::cerr << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        if (!loadResources()) {
            std::cerr << "Failed to load resources!" << std::endl;
            return false;
        }
        
        return true;
    }
    
    bool loadResources() {
        rocketTexture = loadTexture("rocket.png");
        backgroundTexture = loadTexture("background.png");
        explosionTexture = loadTexture("explosion.png");
        
        rocketSound = Mix_LoadWAV("rocket.wav");
        explosionSound = Mix_LoadWAV("explosion.wav");
        backgroundMusic = Mix_LoadMUS("background.mp3");
        
        font = TTF_OpenFont("font.ttf", 24);
        
        return true;
    }
    
    SDL_Texture* loadTexture(const char* filename) {
        SDL_Surface* surface = IMG_Load(filename);
        if (!surface) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            return nullptr;
        }
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        return texture;
    }
    
    void run() {
        Uint32 frameStart;
        int frameTime;
        
        Mix_PlayMusic(backgroundMusic, -1);
        
        while (isRunning) {
            frameStart = SDL_GetTicks();
            
            handleEvents();
            update();
            render();
            
            frameTime = SDL_GetTicks() - frameStart;
            if (FRAME_DELAY > frameTime) {
                SDL_Delay(FRAME_DELAY - frameTime);
            }
        }
    }
    
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    launchRocket(event.button.x, event.button.y);
                }
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r) {
                    resetGame();
                }
            }
        }
    }
    
    void launchRocket(int x, int y) {
        Rocket newRocket;
        newRocket.position = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
        newRocket.velocity = Vector2((x - SCREEN_WIDTH / 2) / 10.0f, 
                                   (y - SCREEN_HEIGHT) / 10.0f);
        newRocket.rotation = atan2(newRocket.velocity.y, newRocket.velocity.x) * 180 / M_PI;
        newRocket.active = true;
        newRocket.type = rand() % 3;
        
        rockets.push_back(newRocket);
        
        Mix_PlayChannel(-1, rocketSound, 0);
    }
    
    void update() {
        for (auto& rocket : rockets) {
            if (rocket.active) {
                rocket.position.x += rocket.velocity.x;
                rocket.position.y += rocket.velocity.y;
                
                rocket.velocity.y += 0.1f;
                
                if (rocket.position.y > SCREEN_HEIGHT || 
                    rocket.position.x < 0 || 
                    rocket.position.x > SCREEN_WIDTH) {
                    rocket.active = false;
                }
            }
        }
        
        rockets.erase(std::remove_if(rockets.begin(), rockets.end(),
            [](const Rocket& r) { return !r.active; }), rockets.end());
    }
    
    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        
        for (const auto& rocket : rockets) {
            if (rocket.active) {
                SDL_Rect destRect = {
                    static_cast<int>(rocket.position.x - 16),
                    static_cast<int>(rocket.position.y - 16),
                    32, 32
                };
                
                SDL_RenderCopyEx(renderer, rocketTexture, NULL, &destRect,
                                rocket.rotation + 90, NULL, SDL_FLIP_NONE);
            }
        }
        
        drawText("Score: " + std::to_string(score), 10, 10, {255, 255, 255, 255});
        
        SDL_RenderPresent(renderer);
    }
    
    void drawText(const std::string& text, int x, int y, SDL_Color color) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) return;
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    
    void resetGame() {
        rockets.clear();
        score = 0;
        gameOver = false;
    }
    
    void cleanup() {
        SDL_DestroyTexture(rocketTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(explosionTexture);
        
        Mix_FreeChunk(rocketSound);
        Mix_FreeChunk(explosionSound);
        Mix_FreeMusic(backgroundMusic);
        
        TTF_CloseFont(font);
        
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    Game game;
    
    if (!game.initialize()) {
        std::cerr << "Game initialization failed!" << std::endl;
        return 1;
    }
    
    game.run();
    game.cleanup();
    
    return 0;
}
