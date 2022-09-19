#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "SpaceShip.hpp"

class Laser : public GameObject, public Collidable  {
public:
    Laser(const sre::Sprite &sprite, glm::vec2 position, glm::vec2 velocity, glm::vec2 direction);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    glm::vec2 velocity;
private:
    glm::vec2 winSize;
    uint64_t spawnTime;
};

