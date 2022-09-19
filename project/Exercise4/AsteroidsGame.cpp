//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#include <ctime>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#include "Asteroid.hpp"
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "Laser.hpp"
#include "SpaceShip.hpp"

using namespace sre;

sre::Sprite getAsteroidOfSize(AsteroidSize size, std::shared_ptr<SpriteAtlas> atlas) {
    auto names = atlas->getNames();
    std::string sizeString;
    switch (size) {
        case TINY:
            sizeString = "tiny";
            break;
        case SMALL:
            sizeString = "small";
            break;
        case MEDIUM:
            sizeString = "med";
            break;
        case LARGE:
            sizeString = "big";
            break;
    }
    names.erase(std::remove_if(names.begin(), names.end(), [size, sizeString](std::string name) {
                    return strncmp(name.c_str(), "Meteors/", 8) || name.find(sizeString) == std::string::npos;
                }),
                names.end());
    int pick = rand() % names.size();

    return atlas->get(names[pick]);
}

AsteroidsGame::AsteroidsGame() {
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING).withSdlWindowFlags(SDL_WINDOW_OPENGL);

    time_t t;  // random seed based on time
    srand((unsigned)time(&t));

    atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");
    auto spaceshipSprite = atlas->get("playerShip1_green.png");
    gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite));

    for (int i = 0; i < 5; i++) {
        auto size = (AsteroidSize)(i % (LARGE + 1));
        auto astroidSprite = getAsteroidOfSize(size, atlas);
        gameObjects.push_back(std::make_shared<Asteroid>(astroidSprite, size));
    }

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event) {
        keyEvent(event);
    };

    r.frameRender = [&]() {
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::update(float deltaTime) {
    for (int i = 0; i < gameObjects.size(); i++) {
        for (int j = 0; j < gameObjects.size(); j++) {
            if (j == i) continue;

            auto a = gameObjects[i];
            auto colA = std::dynamic_pointer_cast<Collidable>(a);
            auto b = gameObjects[j];
            auto colB = std::dynamic_pointer_cast<Collidable>(b);
            if (colA && colB && glm::distance(a->position, b->position) <= colA->getRadius() + colB->getRadius()) {
                colA->onCollision(b);
                colB->onCollision(a);
            }
        }
    }

    gameObjects.erase(std::remove_if(
                          gameObjects.begin(),
                          gameObjects.end(),
                          [deltaTime](std::shared_ptr<GameObject> gameObject) {
                              gameObject->update(deltaTime);
                              return gameObject->cleanUp;
                          }),
                      gameObjects.end());
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius) {
    float quarterPi = glm::quarter_pi<float>();
    for (float f = 0; f < glm::two_pi<float>(); f += quarterPi) {
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f) * radius,
                                  position.y + sinf(f) * radius,
                                  0});
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f + quarterPi) * radius,
                                  position.y + sinf(f + quarterPi) * radius,
                                  0});
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
                          .withCamera(camera)
                          .withClearColor(true, {.20, .60, .86, 1})
                          .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles) {
        std::vector<glm::vec3> lines;
        for (auto& go : gameObjects) {
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr) {
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i", score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event& event) {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->onKey(event);
        auto spaceShip = std::dynamic_pointer_cast<SpaceShip>(gameObjects[i]);
        if (spaceShip && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            spawnLaser(spaceShip);
        }
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
        debugCollisionCircles = !debugCollisionCircles;
    }
}

void AsteroidsGame::spawnLaser(std::shared_ptr<SpaceShip> spaceShip) {
    auto laserSprite = atlas->get("Lasers/laserBlue01.png");
    auto position = spaceShip->position;
    glm::vec2 direction = glm::rotateZ(glm::vec3(0, 1.0f, 0), glm::radians(spaceShip->rotation));
    position += direction * spaceShip->getRadius();
    gameObjects.push_back(std::make_shared<Laser>(laserSprite, position, direction));
}

int main() {
    new AsteroidsGame();
    return 0;
}