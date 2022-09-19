//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Laser.hpp"
#include "sre/Renderer.hpp"
#include <chrono>

using namespace std::chrono;

uint64_t currentTime() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

Laser::Laser(const sre::Sprite &sprite, glm::vec2 position, glm::vec2 direction) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    this->radius = 7;
    this->position = position;
    this->velocity = direction * 200.0f;
    this->rotation = glm::degrees(glm::atan(velocity.y, velocity.x)) - 90.0f;
    spawnTime = currentTime();
}

void Laser::update(float deltaTime) {
    position += velocity * deltaTime;

    // wrap around
    if (position.x < 0){
        position.x += winSize.x;
    } else if (position.x > winSize.x){
        position.x -= winSize.x;
    }
    if (position.y < 0){
        position.y += winSize.y;
    } else if (position.y > winSize.y){
        position.y -= winSize.y;
    }

    if(currentTime() - spawnTime > 1000) {
        cleanUp = true;
    }
}

void Laser::onCollision(std::shared_ptr<GameObject> other) {
    std::cout << "Collision of Laser\n";
}

void Laser::onKey(SDL_Event &keyEvent) {
    
}
