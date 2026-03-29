#include <iostream>

#include "ke_engine.hpp"

constexpr unsigned int FPS = 60;

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
    raylib::Window w(screenWidth, screenHeight, "Text Adventure");

    KeEngine engine(true);

    engine.Init();
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