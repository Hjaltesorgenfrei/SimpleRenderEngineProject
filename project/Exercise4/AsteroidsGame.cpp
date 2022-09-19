//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include <iostream>

using namespace sre;

AsteroidsGame::AsteroidsGame() {
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
     .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    time_t t;                                               // random seed based on time
    srand((unsigned) time(&t));

    atlas = SpriteAtlas::create("asteroids.json","asteroids.png");

    for (const auto& s : atlas->getNames()) {
        std::cout << s << "\n";
    }
    auto spaceshipSprite = atlas->get("playerShip1_green.png");
    gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite));

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event){
        keyEvent(event);
    };

    r.frameRender = [&](){
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::update(float deltaTime) {
	for (int i = 0; i < gameObjects.size();i++) {
		gameObjects[i]->update(deltaTime);
    }
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius){
    float quaterPi = glm::quarter_pi<float>();
    for (float f = 0;f<glm::two_pi<float>();f += quaterPi){
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f)*radius,
                                  position.y + sinf(f)*radius,
                                  0
        });
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f+quaterPi)*radius,
                                  position.y + sinf(f+quaterPi)*radius,
                                  0
        });
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles){
        std::vector<glm::vec3> lines;
        for (auto & go : gameObjects){
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr){
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2-100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
        debugCollisionCircles = !debugCollisionCircles;
    }
}

int main(){
    new AsteroidsGame();
    return 0;
}