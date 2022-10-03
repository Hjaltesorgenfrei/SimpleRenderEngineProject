//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "WorldMap.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace std;

void WorldMap::loadMap(std::string filename) {
    // todo implement json loading this instead following

    using namespace rapidjson;
    ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    const Value& rows = d["tileMap"];
    for (SizeType i = 0; i < rows.Size(); i++) {
        vector<int> rowValues;
        for (SizeType j = 0; j < rows[i].Size(); j++) {
            rowValues.push_back(rows[i][j].GetInt());
        }
        values.push_back(rowValues);
    }
    
    const Value& spawn = d["spawn"];
    startingPosition.x = spawn["x"].GetFloat();
    startingPosition.y = spawn["y"].GetFloat();
    startingRotation = spawn["angle"].GetFloat();

    const Value& floor = d["floorColor"];
    const Value& ceil = d["ceilColor"];
    ceilColor = glm::vec4(floor[0].GetFloat(), floor[1].GetFloat(), floor[2].GetFloat(), floor[3].GetFloat());
    floorColor = glm::vec4(ceil[0].GetFloat(), ceil[1].GetFloat(), ceil[2].GetFloat(), ceil[3].GetFloat());
}

int WorldMap::getTile(int x, int y) {
    return values.at(y).at(x);
}

int WorldMap::getWidth() {
    return values[0].size();
}

int WorldMap::getHeight() {
    return values.size();
}

glm::vec2 WorldMap::getStartingPosition() {
    return startingPosition;
}

float WorldMap::getStartingRotation() {
    return startingRotation;
}

glm::vec4 WorldMap::getFloorColor() {
    return floorColor;
}

glm::vec4 WorldMap::getCeilColor() {
    return ceilColor;
}
