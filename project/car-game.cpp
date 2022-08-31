#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "sre/Texture.hpp"
#include "sre/Inspector.hpp"
#include "glm/gtc/random.hpp"
#include "glm/ext.hpp"
#include "Box2D/Box2D.h"

using namespace sre;
using namespace std;

struct Player{
    bool forward, backwards, left, right;
    b2Body* body;
    shared_ptr<SpriteAtlas> atlas;
    float speed = 10000, rotationSpeed = 2000;
};

// Captures debug information from Box2D as lines (polygon fill and color is discarded)
class Box2DDebugDraw : public b2Draw{
public:
    Box2DDebugDraw(){
        SetFlags( b2Draw::e_shapeBit );
    }
    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        for (int i=0;i<vertexCount;i++){
            lines.push_back({vertices[i].x,vertices[i].y,0});
            lines.push_back({vertices[(i+1)%vertexCount].x,vertices[(i+1)%vertexCount].y,0});
        }
    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        DrawPolygon(vertices, vertexCount, color);
    }

    void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) override {
        glm::vec3 c {center.x,center.y,0};
        for (int i=0;i<16;i++){
            float v = i*glm::two_pi<float>()/16;
            float v1 = (i+1)*glm::two_pi<float>()/16;
            lines.push_back(c+glm::vec3{sin(v ) , cos(v ) , 0} * radius);
            lines.push_back(c+glm::vec3{sin(v1) , cos(v1) , 0} * radius);
        }
    }

    void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) override {
        DrawCircle(center, radius, color);
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {
        lines.push_back({p1.x,p1.y,0});
        lines.push_back({p2.x,p2.y,0});
    }

    void DrawTransform(const b2Transform &xf) override {}

    void DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) override {}

    void clear(){
        lines.clear();
    }
    const vector<glm::vec3>& getLines(){
        return lines;
    }
private:
    vector<glm::vec3> lines;
};

class Box2D {
public:
    Box2D()
    :m_world({0.0f,0.0f})
    {

        m_world.SetDebugDraw(&debugDraw);                                           // enable debug draw (line drawing on colliders)

        r.init();
        r.setWindowTitle("Box2D example. Drag mouse to spawn boxes.");

        camera.setWindowCoordinates();

        atlas = SpriteAtlas::createSingleSprite(Texture::getWhiteTexture());        // Create white test sprite
        player.atlas = SpriteAtlas::create("project_data/car-game.json",Texture::create()
            .withFile( "project_data/car-game.png")
            .withFilterSampling(false)
            .build() );
        b2BodyDef myBodyDef;                                                        // Setup phys
        myBodyDef.type = b2_staticBody; //this will be a static body (does not move)
        myBodyDef.position.Set(0, 0); //set the starting position
        auto m_groundBody = m_world.CreateBody(&myBodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(10000,10);

        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 1;
        m_groundBody->CreateFixture(&boxFixtureDef);

        r.frameUpdate = [&](float deltaTime){                                       // Update physics simulation before rendering
            float fixedDeltaTime = 0.016f;
            int32 velocityIterations = 8;                                           //how strongly to correct velocity
            int32 positionIterations = 3;        
            applySpeed(deltaTime);
            m_world.Step(fixedDeltaTime,velocityIterations,positionIterations);
        };
        r.frameRender = [&](){
            render();
        };

        r.mouseEvent = [&](SDL_Event& e){
            mouseEvent(e);
        };

        // spawn a few boxes
        spawnBox(500, 500, 45);
        spawnPlayer(500, 500, 0);


        r.startEventLoop();
    }

    void render(){
        auto renderPass = RenderPass::create()
                .withCamera(camera)
                .withClearColor(true,{.92f, .92f, .92f, 1.0f})
                .build();

        auto sprite = atlas->get("sprite");
        sprite.setScale({10,10});
        sprite.setPosition({50,50});
        for (int i=0;i<sprites.size();i++){
            auto pos = physicsEntities[i]->GetPosition();
            auto rotation = glm::degrees(physicsEntities[i]->GetAngle());
            sprites[i].setPosition({pos.x,pos.y});
            sprites[i].setRotation(rotation);
        }
        auto sb = SpriteBatch::create().addSprites(sprites.begin(), sprites.end()).build();
        renderPass.draw(sb);
        if (ImGui::SliderFloat("Forward speed", &player.speed, 1000, 10000)){
            cout << "Changed Speed\n";
        }
        if (ImGui::SliderFloat("Rotation speed", &player.rotationSpeed, 1000, 5000)){
            cout << "Changed RotationSpeed\n";
        }

        // debug draw
        m_world.DrawDebugData();
        renderPass.drawLines(debugDraw.getLines(),{0,0,0,1});
        debugDraw.clear();
    }

    void mouseEvent(SDL_Event& e){
        if (ImGui::IsMouseHoveringAnyWindow()) {
            return;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN){
            spawn = true;
        }
        if (e.type == SDL_MOUSEBUTTONUP){
            spawn = false;
        }
        if (e.type == SDL_MOUSEMOTION && spawn){
            auto r = Renderer::instance;
            glm::vec2 pos{e.motion.x, r->getWindowSize().y - e.motion.y};

            // convert to pixel coordinates (for HighDPI displays)
            pos /= r->getWindowSize();
            pos *= r->getDrawableSize();

            int mouseX = static_cast<int>(pos.x);
            int mouseY = static_cast<int>(pos.y);

            spawnBox(mouseX, mouseY, 0);
        }
    }

    void applySpeed(float deltaTime) {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        //continuous-response keys
        player.left = keystate[SDL_SCANCODE_LEFT];
        player.right = keystate[SDL_SCANCODE_RIGHT];
        player.forward = keystate[SDL_SCANCODE_UP];
        player.backwards = keystate[SDL_SCANCODE_DOWN];

        float rotation = 0;
        if (player.left){
            rotation += 4;
        }
        if (player.right){
            rotation -= 4;
        }
        b2Body* body = player.body;
        body->SetAwake(true);
        const int speed = player.speed;
        body->ApplyAngularImpulse(rotation * body->GetMass() * deltaTime * player.rotationSpeed, true);
        if (player.forward ){
            auto angle = body->GetAngle();
            b2Vec2 impulse{glm::cos(angle) * speed * deltaTime * 1000, glm::sin(angle) * speed * deltaTime * 1000};
            body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        }
    }

    void spawnPlayer(int posX, int posY, float angle) {
        auto sprite = player.atlas->get("simple-travel-car-top_view.svg");
        const float size = 0.1f;
        sprite.setScale({size,size});
        sprite.setPosition({posX,posY});
        sprite.setColor(glm::vec4{glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f),
                                  glm::linearRand<float>(0.0f, 1.0f), 1.0f});
        sprites.push_back(sprite);

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;                //this will be a dynamic body
        myBodyDef.position.Set(posX,posY);              //set the starting position
        myBodyDef.angle = angle;                        //set the starting angle
        auto dynBody = m_world.CreateBody(&myBodyDef);

        b2PolygonShape boxShape;
        auto sprite_size = sprite.getSpriteSize();
        boxShape.SetAsBox(sprite_size.x * size / 2,sprite_size.y * size / 2);

        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.restitution = 0.8;   // elasticity [0;1]
        boxFixtureDef.density = 1;         // weight
        dynBody->CreateFixture(&boxFixtureDef);
        physicsEntities.push_back(dynBody);
        player.body = dynBody;
        player.body->SetAngularDamping(5);
        player.body->SetLinearDamping(1);
    }

    void spawnBox(int posX, int posY, float angle) {
        auto sprite = atlas->get("sprite");
        const int size = 20;
        sprite.setScale({size,size});
        sprite.setPosition({posX,posY});
        sprite.setColor(glm::vec4{glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f),
                                  glm::linearRand<float>(0.0f, 1.0f), 1.0f});
        sprites.push_back(sprite);

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;                //this will be a dynamic body
        myBodyDef.position.Set(posX,posY);              //set the starting position
        myBodyDef.angle = angle;                        //set the starting angle
        auto dynBody = m_world.CreateBody(&myBodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(size,size);

        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.restitution = 0.8;   // elasticity [0;1]
        boxFixtureDef.density = 1;         // weight
        dynBody->CreateFixture(&boxFixtureDef);
        physicsEntities.push_back(dynBody);
    }

private:
    b2World m_world;
    std::shared_ptr<SpriteAtlas> atlas;
    std::vector<Sprite> sprites;
    std::vector<b2Body*> physicsEntities;
    SDLRenderer r;
    Camera camera;
    Box2DDebugDraw debugDraw;
    bool spawn = false;
    Player player{};
};

int main() {
    std::cout << "Welcome to Car Game" << std::endl;
    new Box2D();

    return 0;
}