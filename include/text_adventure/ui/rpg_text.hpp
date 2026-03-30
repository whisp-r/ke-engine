#ifndef RPG_TEXT_H
#define RPG_TEXT_H
#pragma once

#include <vector>
#include <array>
#include "raylib-cpp.hpp"

class RPGText : public raylib::Text
{
public:
    RPGText(const std::string &text,
            float fontSize,
            const raylib::Color &color,
            unsigned short speed = 10,
            const raylib::Font &font = GetFontDefault(),
            float spacing = 0);

    void Update(unsigned short delta = 1);
    void Draw(int posX, int posY) const;

    void Skip();
    void Reset();

    void Pause();
    void Resume();
    bool IsPaused() const;
    bool IsComplete() const;

    void SetSpeed(unsigned short newSpeed);

    std::string GetVisibleText() const;

private:
    std::string fullText;

    std::size_t framesCounter = 0;
    unsigned short speed;

    bool isPaused = false;
    bool isComplete = false;
};

#endif