#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

enum AsteroidSize {
    TINY,
    SMALL,
    MEDIUM,
    LARGE
};

class Asteroid : public GameObject, public Collidable  {
public:
    Asteroid(const sre::Sprite &sprite, const AsteroidSize size);
    Asteroid(const sre::Sprite &sprite, const AsteroidSize size, const glm::vec2 position);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    glm::vec2 velocity;
    AsteroidSize size;
private:
    glm::vec2 winSize;
    float rotationSpeed;
};


