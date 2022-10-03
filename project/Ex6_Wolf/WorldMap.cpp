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
    values.push_back( { 0, 0, 0,0} ); // -------> x
    values.push_back( { 0,-1,-1,2} ); // |
    values.push_back( { 0,-1,-1,9} ); // |
    values.push_back( { 0, 0, 0,0} ); // v z
    startingPosition.x = 1.5;
    startingPosition.y = 1.5;
    startingRotation = 22.5;

    // using namespace rapidjson;
    // ifstream fis(filename);
    // IStreamWrapper isw(fis);
    // Document d;
    // d.ParseStream(isw);
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
