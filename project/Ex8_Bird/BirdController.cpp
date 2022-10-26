//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "BirdController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BirdGame.hpp"
#include "SpriteComponent.hpp"
#include "WallComponent.hpp"

BirdController::BirdController(GameObject *gameObject) : Component(gameObject) {
    // initiate bird physics
    phys = gameObject->getComponent<PhysicsComponent>();
    glm::vec2 speed(1.f, 0.f);
    phys->setLinearVelocity(speed);
}

bool BirdController::onKey(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        glm::vec2 jump(0.f, 0.15f);
        phys->addImpulse(jump);
    }
    return false;
}

void BirdController::onCollisionStart(PhysicsComponent *comp) {
    // I really wanted a way to find a component: GameObject.Find("gameState") from unity
    // When i cant have that i just use a raw pointer :)
    if (comp->getGameObject()->getComponent<WallComponent>() && gameState) {
        (*gameState) = GameState::GameOver;
    }
}

void BirdController::onCollisionEnd(PhysicsComponent *comp) {

}


