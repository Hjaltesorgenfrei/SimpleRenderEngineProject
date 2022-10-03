//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace sre;
using namespace glm;

FirstPersonController::FirstPersonController(sre::Camera *camera)
    : camera(camera)
{
    camera->setPerspectiveProjection(45, 0.1f, 1000);
    position = vec3(0, 0, 0);
}

void FirstPersonController::update(float deltaTime)
{
    const auto speed = 5.0f;
    auto direction = rotateY(vec3(0, 0, -1), glm::radians(rotation));

    // This currently allows for the player to move faster when moving diagonally.
    // Instead it should create a vector of moveDirection, normalize it and then multiply with speed and direction
    vec3 moveDirection = vec3(0, 0, 0);

    if (fwd) {
        moveDirection = moveDirection + direction; 
    }
    if (bwd) {
        moveDirection = moveDirection - direction; 
    }
    if (right) {
        auto rightDirection = rotateY(direction, glm::radians(90.0f));
        moveDirection = moveDirection - rightDirection; 
    }
    if (left) {
        auto leftDirection = rotateY(direction, glm::radians(270.0f));
        moveDirection = moveDirection - leftDirection; 
    }
    if (fwd || bwd || right || left) {
        moveDirection = glm::normalize(moveDirection);
        position += speed * deltaTime * moveDirection;
    }
    camera->lookAt(position, position + direction, vec3{0, 1, 0});
}

void FirstPersonController::onKey(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
        case SDLK_w:
        {
            if (event.type == SDL_KEYDOWN) {
                fwd = true;
            }
            else if (event.type == SDL_KEYUP) {
                fwd = false;
            }
            break;
        }
        case SDLK_s:
        {
            if (event.type == SDL_KEYDOWN) {
                bwd = true;
            }
            else if (event.type == SDL_KEYUP) {
                bwd = false;
            }
            break;
        }
        case SDLK_a:
        {
            if (event.type == SDL_KEYDOWN) {
                left = true;
            }
            else if (event.type == SDL_KEYUP) {
                left = false;
            }
            break;
        }
        case SDLK_d:
        {
            if (event.type == SDL_KEYDOWN) {
                right = true;
            }
            else if (event.type == SDL_KEYUP) {
                right = false;
            }
            break;
        }
    }
}

void FirstPersonController::onMouse(SDL_Event &event)
{
    if(event.type == SDL_MOUSEMOTION) {
        rotation -= event.motion.xrel *0.1f;
    }
}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation)
{
    this->position = glm::vec3(position.x, 0, position.y);
    this->rotation = rotation;
}
