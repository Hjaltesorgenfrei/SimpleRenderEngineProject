#ifndef MINESWEEPER_MINESWEEPER_HPP
#define MINESWEEPER_MINESWEEPER_HPP

#include <string>
#include <vector>

enum FieldType {
    Button,             // Field has not been uncovered
    Empty,              // Field has been uncovered but is empty - no bombs nearby
    Empty1,             // Field has been uncovered but is empty - 1 bombs nearby
    Empty2,             // Field has been uncovered but is empty - 2 bombs nearby
    Empty3,             // Field has been uncovered but is empty - 3 bombs nearby
    Empty4,             // Field has been uncovered but is empty - 4 bombs nearby
    Empty5,             // Field has been uncovered but is empty - 5 bombs nearby
    Empty6,             // Field has been uncovered but is empty - 6 bombs nearby
    Empty7,             // Field has been uncovered but is empty - 7 bombs nearby
    Empty8,             // Field has been uncovered but is empty - 8 bombs nearby
    BombExploded,       // Field has been uncovered and contains a exploded bomb
    BombUnexploded,     // Field has been uncovered and contains a unexploded bomb (optionally used at end of game)
    Flag                // Field marked with flag
};

class MineSweeper {
public:
    MineSweeper();

    void initLevel(int size);                           // setup level - randomly position bombs

    void print();                                       // print the level to the console

    FieldType getField(int row, int column);            // Get the field value

    bool isRunning();                                   // Returns false if player has won or lost

    std::string take(int row, int column);              // Uncovers a field
    // If row/column is outside game field, report error
    // If bombsNearby is 0 then take should be invoked on nearby fields
private:
    int numberOfBombs();                                // Returns the number of bombs in the game

    int numberOfHidden();                               // Returns the current number of hidden fields
    
    bool hasBomb(int row, int column);
    bool inside(int row, int column);

    int bombsNearby(int row, int column);               // Returns the number of bombs nearby (rows and column +/- 1)

    std::vector<std::vector<bool>> bombs;
    std::vector<std::vector<FieldType>> fieldTypes;
    int size;
    bool lost = false;
};
#endif