#include "MineSweeper.hpp"

#include <iostream>
#include <random>

using namespace std;

MineSweeper::MineSweeper() {
    // todo implement
}

int randomNumber(int min, int max) {
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min, max); // define the range
    return distr(gen);
}

void MineSweeper::initLevel(int size) {
    this->size = size;
    fieldTypes = vector<vector<FieldType>>(size, vector<FieldType>(size, FieldType::Button));
    bombs = vector<vector<bool>>(size, vector<bool>(size, false));
    int bombsLeft = (size * size) / 5;
    while (bombsLeft) {
        int x = randomNumber(0, size - 1);
        int y = randomNumber(0, size - 1);
        if (!bombs[y][x]) {
            bombs[y][x] = true;
            bombsLeft--;
        }
    }
}

string fieldTypeToString(FieldType field) {
    switch (field) {
        case Button:
            return "X";
        case Empty:
            return "E";
        case Empty1:
            return "1";
        case Empty2:
            return "2";
        case Empty3:
            return "3";
        case Empty4:
            return "4";
        case Empty5:
            return "5";
        case Empty6:
            return "6";
        case Empty7:
            return "7";
        case Empty8:
            return "8";
        case BombExploded:
            return "B";
        case BombUnexploded:
            return "b";
        case Flag:
            return "F";
        default:
            return "?";
    }
}

void MineSweeper::print() {
    cout << "Hidden: " << numberOfHidden() << ", Bombs:" << numberOfBombs() << "\n";
    int y = 0;
    for (auto row : fieldTypes) {
        cout << y++ << " ";
        for (auto field : row) {
            cout << fieldTypeToString(field) << " ";
        }
        cout << "\n";
    }
    cout << "  ";
    for (int x = 0; x < size; x++) {
        cout << x << " ";
    }
    cout << "\n";
    if (!isRunning()) {
        if (!lost) {
            cout << "You win!\n";
        }
        else {
            cout << "You loose fool!\n";
        }
    }
}

FieldType MineSweeper::getField(int row, int column) {
    return fieldTypes[row][column];
}

bool MineSweeper::isRunning() {
    for (auto row : fieldTypes) {
        for (auto field : row) {
            if (field == FieldType::BombExploded) {
                return false;
            }
        }
    }
    if (numberOfHidden() == 0) {
        return false;
    }
    return true;
}

std::string MineSweeper::take(int row, int column) {
    if (!inside(row, column)) {
        return "Location not inside: " + to_string(row) + ", " + to_string(column);
    }
    if (fieldTypes[row][column] != Button) {
        return "Location already choosen before: " + to_string(row) + ", " + to_string(column);
    }

    if (hasBomb(row, column)) {
        fieldTypes[row][column] = BombExploded;
        lost = true;
        for (int y = 0; y < size; y++) {
            for(int x = 0; x < size; x++) {
                take(y, x);
            }
        }
    } else {
        FieldType newField;
        switch (bombsNearby(row, column)) {
            case 0:
                newField = Empty;
                break;
            case 1:
                newField = Empty1;
                break;
            case 2:
                newField = Empty2;
                break;
            case 3:
                newField = Empty3;
                break;
            case 4:
                newField = Empty4;
                break;
            case 5:
                newField = Empty5;
                break;
            case 6:
                newField = Empty6;
                break;
            case 7:
                newField = Empty7;
                break;
            case 8:
                newField = Empty8;
                break;

            default:
                throw std::runtime_error("Got too many bombs");
        }
        fieldTypes[row][column] = newField;
    }
    if (fieldTypes[row][column] == Empty) {
        take(row - 1, column - 1);
        take(row - 1, column);
        take(row - 1, column + 1);
        take(row, column - 1);
        take(row, column + 1);
        take(row + 1, column - 1);
        take(row + 1, column);
        take(row + 1, column + 1);
    }
    return "Took " + to_string(row) + ", " + to_string(column);
}

int MineSweeper::numberOfBombs() {
    int count = 0;
    for (auto row : bombs) {
        for (auto isBomb : row) {
            count += (isBomb);
        }
    }
    return count;
}

int MineSweeper::numberOfHidden() {
    int count = 0;
    for (auto row : fieldTypes) {
        for (auto field : row) {
            count += (field == FieldType::Button);
        }
    }
    return count;
}

bool MineSweeper::inside(int row, int column) {
    return row >= 0 && row < size && column >= 0 && column < size;
}

bool MineSweeper::hasBomb(int row, int column) {
    return inside(row, column) && bombs[row][column];
}

int MineSweeper::bombsNearby(int row, int column) {
    int count = 0;
    count += hasBomb(row - 1, column - 1);
    count += hasBomb(row - 1, column);
    count += hasBomb(row - 1, column + 1);
    count += hasBomb(row, column - 1);
    count += hasBomb(row, column + 1);
    count += hasBomb(row + 1, column - 1);
    count += hasBomb(row + 1, column);
    count += hasBomb(row + 1, column + 1);
    return count;
}
