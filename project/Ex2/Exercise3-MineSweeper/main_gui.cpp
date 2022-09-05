//
// Created by Morten Nobel-Jørgensen on 18/07/2017.
//

#include <iostream>
#include <vector>
#include <fstream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "sre/SDLRenderer.hpp"

#include "MineSweeper.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <sre/ModelImporter.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sre/SpriteAtlas.hpp>
#include <sre/Inspector.hpp>


using namespace sre;

class MineSweeperGUI {
public:
    MineSweeperGUI()
    {
        mineSweeper.initLevel(size);
        r.setWindowSize({400,500});
        r.init();

        atlas = SpriteAtlas::create("minesweeper.json","minesweeper.png");
        sprites.push_back(atlas->get("button.png")); // Button,
        sprites.push_back(atlas->get("empty.png")); // Empty,
        sprites.push_back(atlas->get("1.png")); // Empty1,
        sprites.push_back(atlas->get("2.png")); // Empty2,
        sprites.push_back(atlas->get("3.png")); // Empty3,
        sprites.push_back(atlas->get("4.png")); // Empty4,
        sprites.push_back(atlas->get("5.png")); // Empty5,
        sprites.push_back(atlas->get("6.png")); // Empty6,
        sprites.push_back(atlas->get("7.png")); // Empty7,
        sprites.push_back(atlas->get("8.png")); // Empty8,
        sprites.push_back(atlas->get("bomb-exploded.png")); // BombExploded,
        sprites.push_back(atlas->get("bomb-unexploded.png")); // BombUnexploded

        camera.setWindowCoordinates();

        r.mouseEvent = [&](SDL_Event event){
            mouseEvent(event);
        };
        r.frameRender = [&](){
            render();

        };

        r.startEventLoop();
    }

    void mouseEvent(SDL_Event mouseEvent){
        if (mouseEvent.type == SDL_MOUSEBUTTONDOWN){
            int x = mouseEvent.motion.x/spriteSize;
            int y = (Renderer::instance->getWindowSize().y-mouseEvent.motion.y)/spriteSize;
            if (x >=0 && x < size && y>=0 && y < size){
                mineSweeper.take(x,y);
            }
        }
    }

    void render(){
        auto renderPass = RenderPass::create()
                .withCamera(camera)
                .withClearColor(true, {.3, .3, 1, 1})
                .build();

        auto sb = SpriteBatch::create();
        for (int x=0;x<size;x++){
            for (int y=0;y<size;y++) {
                auto sprite = sprites[mineSweeper.getField(x,y)];
                sprite.setPosition({(x+0.5f)*spriteSize,(y+0.5f)*spriteSize});
                sb.addSprite(sprite);
            }
        }
        auto sbo = sb.build();
        renderPass.draw(sbo);

        ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(Renderer::instance->getWindowSize().x, 70), ImGuiSetCond_Always);
        ImGui::Begin("", nullptr,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);
        ImGui::InputInt("Size", &nextSize);
        if (ImGui::Button("Restart")){
            size = nextSize;
            mineSweeper.initLevel(size);
        }
        ImGui::End();
    }
private:
    int nextSize = 10;
    int size = 10;
    int spriteSize = 16;
    MineSweeper mineSweeper;
    std::shared_ptr<SpriteAtlas> atlas;
    std::vector<sre::Sprite> sprites;
    SDLRenderer r;
    Camera camera;
    std::shared_ptr<SpriteBatch> world;
};

int main() {
    new MineSweeperGUI();
    return 0;
}
