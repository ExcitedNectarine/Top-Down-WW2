#pragma once

#include "SDL.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

/**
* This namespace contains miscellaneous helpful functions.
*/
namespace Tools
{
    struct FloatVector
    {
        float x;
        float y;
    };

    /**
    * This function splits a string into a vector of strings. The string is split
    * based on the delimeter.
    */
    std::vector<std::string> splitText(const std::string& text, const char delimeter);

    /**
    * This function returns the angle between two points.
    */
    int angleBetweenPoints(const int x1, const int y1, const int x2, const int y2);

    /**
    * This function returns a normalized vector.
    */
    FloatVector normalizeVector(const FloatVector& vector);

    /**
    * This function returns a random float between "from" and "to".
    */
    float randomFloat(const float from, const float to);

    /**
    * This function returns a random int between "from" and "to".
    */
    int randomInt(const int from, const int to);
}