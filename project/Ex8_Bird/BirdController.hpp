#pragma once

#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "GameState.hpp"

class BirdController : public Component {
public:
    explicit BirdController(GameObject *gameObject);

    bool onKey(SDL_Event &event) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    std::shared_ptr<GameState> gameState;
private:
    std::shared_ptr<PhysicsComponent> phys;
};
