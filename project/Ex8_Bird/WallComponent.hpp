#pragma once

#include "Component.hpp"

class WallComponent : public Component {
public:
    WallComponent(GameObject *gameObject) : Component(gameObject) {}
};