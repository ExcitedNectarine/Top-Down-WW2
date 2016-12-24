#include "Tools.h"

/**
* This namespace contains miscellaneous helpful functions.
*/
namespace Tools
{
    /**
    * This function splits a string into a vector of strings. The string is split
    * based on the delimeter.
    */
    std::vector<std::string> splitText(const std::string& text, const char delimeter)
    {
        std::vector<std::string> split;
        std::stringstream ss(text);
        std::string item;

        while (std::getline(ss, item, delimeter))
        {
            split.push_back(item);
        }

        return split;
    }

    /**
    * This function returns the angle between two points.
    */
    int angleBetweenPoints(const int x1, const int y1, const int x2, const int y2)
    {
        return static_cast<int>(((std::atan2(y1 - y2, x1 - x2) * 180.0000) / 3.1416));
    }

    /**
    * This function returns a normalized vector.
    */
    FloatVector normalizeVector(const FloatVector& vector)
    {
        float length = std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
        return { vector.x / length, vector.y / length };
    }

    /**
    * This function returns a random float between "from" and "to".
    */
    float randomFloat(const float from, const float to)
    {
        static std::default_random_engine engine(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_real_distribution<float> dist(from, to);
        return dist(engine);
    }

    /**
    * This function returns a random int between "from" and "to".
    */
    int randomInt(const int from, const int to)
    {
        static std::default_random_engine engine(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<int> dist(from, to);
        return dist(engine);
    }
}