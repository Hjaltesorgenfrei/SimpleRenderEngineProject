//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Asteroid.hpp"
#include "sre/Renderer.hpp"

#define	RAND_LIMIT	32767

inline float RandomFloat(float lo, float hi)
{
	float r = (float)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

Asteroid::Asteroid(const sre::Sprite &sprite, const AsteroidSize size) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    this->size = size;
    winSize = sre::Renderer::instance->getDrawableSize();
        switch (size) {
        case TINY:
            radius = 4;
            break;
        case SMALL:
            radius = 8;
            break;
        case MEDIUM:
            radius = 14;
            break;
        case LARGE:
            radius = 24;
            break;
    }
    position = glm::vec2(RandomFloat(0.0f, winSize.x), RandomFloat(0.0f, winSize.y));
    velocity = glm::vec2(RandomFloat(-1.0f, 1.0f),RandomFloat(-1.0f, 1.0f)) * 50.0f;
    rotationSpeed = RandomFloat(-1.0f, 1.0f) * 50.0f;
}

Asteroid::Asteroid(const sre::Sprite &sprite, const AsteroidSize size, const glm::vec2 position) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    this->size = size;
    winSize = sre::Renderer::instance->getDrawableSize();
        switch (size) {
        case TINY:
            radius = 4;
            break;
        case SMALL:
            radius = 8;
            break;
        case MEDIUM:
            radius = 14;
            break;
        case LARGE:
            radius = 24;
            break;
    }
    this->position = position;
    velocity = glm::vec2(RandomFloat(-1.0f, 1.0f),RandomFloat(-1.0f, 1.0f)) * 50.0f;
    rotationSpeed = RandomFloat(-1.0f, 1.0f) * 50.0f;
}

void Asteroid::update(float deltaTime) {
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

    rotation += deltaTime * rotationSpeed;
}

void Asteroid::onCollision(std::shared_ptr<GameObject> other) {
    cleanUp = true;
}

void Asteroid::onKey(SDL_Event &keyEvent) {
    
}
