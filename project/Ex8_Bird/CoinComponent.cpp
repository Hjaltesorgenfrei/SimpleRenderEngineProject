#include "CoinComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

CoinComponent::CoinComponent(GameObject *gameObject) : Component(gameObject) {

}

void CoinComponent::onCollisionStart(PhysicsComponent *phys) {
    gameObject->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(-10000.0f, -10000.0f));
}