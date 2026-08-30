#pragma once

#include <raylib.h>

// Guarda la apariencia elegida y sabe dibujar el avatar en diferentes poses.
class Character
{
public:
    Character();
    void Draw(Vector2 position, float scale, int pose, float time, bool locked = false) const;
    void SetBodyColor(int colorIndex);
    void SetAccessory(int accessoryIndex);
    int GetBodyColor() const;
    int GetAccessory() const;

private:
    void DrawAccessory(Vector2 body, Vector2 head, float scale) const;
    int bodyColor;
    int accessory;
};
