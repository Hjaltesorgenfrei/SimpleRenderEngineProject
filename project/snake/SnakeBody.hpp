#pragma once

#include "SnakeGUI.hpp"

class SnakeBody {
public:
    SnakeBody(int x, int y);
    ~SnakeBody();
    void moveTo(int x, int y);
    bool collide(int x, int y);
    std::shared_ptr<SnakeBody> next = nullptr;
private:
    int x;
    int y;
    static int totalSnakeBodies;
    friend class Snake;
    friend class SnakeGUI;
};

