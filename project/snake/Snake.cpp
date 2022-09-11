#include "Snake.hpp"
#include "SnakeBody.hpp"

Snake::Snake() {
}

void Snake::move(){         
    int prevPosX = snakeHead->x;
    int prevPosY = snakeHead->y;
    snakeHead->moveTo(prevPosX + moveX, prevPosY + moveY);
    auto snakePart = snakeHead;
    while(snakePart->next && (snakePart = snakePart->next)) {
        int tempPosX = snakePart->x;
        int tempPosY = snakePart->y;
        snakePart->moveTo(prevPosX, prevPosY);
        prevPosX = tempPosX;
        prevPosY = tempPosY;
    }
    if(leftToGrow) {
        snakePart->next = std::make_shared<SnakeBody>(prevPosX,prevPosY);
        leftToGrow--;
    }
}

void Snake::setMoveDirection(int moveX, int moveY) {                // This is called whenever one arrow key is pressed. Values are [-1,0,1] for x and y.
    if (!(this->moveX + moveX) && !(this->moveY + moveY)) {
        return;
    }
    this->moveX = moveX;
    this->moveY = moveY;
}

void Snake::init(int initialLength, int x, int y, int moveX, int moveY) {   // This is called every time the game starts or restarts. Make sure not to leak resources.
    length = initialLength;
    this->moveX = moveX;
    this->moveY = moveY;
    snakeHead = std::make_shared<SnakeBody>(x,y);
    leftToGrow = initialLength;
}

void Snake::grow() {                                                // Should increment the length and append a SnakeBody
    leftToGrow++;
}

bool Snake::collide(int x, int y) {                                 // Returns true if one of the SnakeBody objects has the position (x,y)
    return snakeHead->collide(x, y);
}
