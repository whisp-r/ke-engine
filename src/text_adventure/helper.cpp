#include "helper.hpp"

#include <stdexcept>
#include <iostream>

raylib::Color LoadColorFromJSON(const json &colorData)
{
    // Array format: [r, g, b] or [r, g, b, a]
    if (colorData.is_array() && colorData.size() >= 3) {
        int r = std::clamp(colorData[0].get<int>(), 0, 255);
        int g = std::clamp(colorData[1].get<int>(), 0, 255);
        int b = std::clamp(colorData[2].get<int>(), 0, 255);
        int a = (colorData.size() > 3) ? std::clamp(colorData[3].get<int>(), 0, 255) : 255;
        
        return raylib::Color(r, g, b, a);
    }
    
    throw std::invalid_argument("[ERROR][Helper] Color must be array [R,G,B] or [R,G,B,A]");
}

raylib::Color GetColorFromJSON(const json &colorData, const raylib::Color &fallback)
{

    if (!colorData.is_array() || colorData.size() < 3) {
        return fallback;
    }
    
    try {
        int r = colorData[0].get<int>();
        int g = colorData[1].get<int>();
        int b = colorData[2].get<int>();
        int a = (colorData.size() > 3) ? colorData[3].get<int>() : 255;
        
        // Clamp to valid range
        r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        b = (b < 0) ? 0 : (b > 255) ? 255 : b;
        a = (a < 0) ? 0 : (a > 255) ? 255 : a;
        
        return raylib::Color(r, g, b, a);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR][Helper] Color parsing failed, using fallback: " << e.what() << std::endl;
        return fallback;
    }
}
