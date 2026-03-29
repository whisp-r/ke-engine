#include <raylib-cpp.hpp>

#include "ke_engine.hpp"

#include <iostream>

constexpr unsigned int FPS = 60;

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
    raylib::Window w(screenWidth, screenHeight, "Text Adventure");

    Engine engine(true);
    if (!engine.Init())
    {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }

    engine.StartStory("start");

    SetTargetFPS(FPS);

    while (!w.ShouldClose())
    {
        // UPDATE
        engine.Update();
        engine.HandleInput();

        // DRAW
        w.BeginDrawing();
        w.ClearBackground(RAYWHITE);
        // ui.Draw()

        engine.DrawUI();

        w.EndDrawing();
    }
}