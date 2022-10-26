#pragma once

#include "Component.hpp"

class CoinComponent : public Component {
public:
    explicit CoinComponent(GameObject *gameObject);

    void onCollisionStart(PhysicsComponent *comp) override;
};