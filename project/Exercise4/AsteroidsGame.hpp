#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "SpaceShip.hpp"

class GameObject;

class AsteroidsGame {
public:
    AsteroidsGame();
private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);

    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::shared_ptr<SpaceShip> spaceShip;
    bool debugCollisionCircles = false;
    int score = 0;
};


