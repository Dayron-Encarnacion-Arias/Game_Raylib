#pragma once

#include <raylib.h>

enum class CharacterType
{
    Operator = 0,
    CrimsonCadet = 1
};

// Guarda la apariencia elegida y sabe dibujar el avatar en diferentes poses.
class Character
{
public:
    Character();
    void Draw(Vector2 position, float scale, int pose, float time, bool locked = false) const;
    void SetBodyColor(int colorIndex);
    void SetAccessory(int accessoryIndex);
    void SetType(int typeIndex);
    int GetBodyColor() const;
    int GetAccessory() const;
    int GetType() const;

private:
    void DrawOperator(Vector2 body, float scale, int pose, float time, bool locked) const;
    void DrawCrimsonCadet(Vector2 body, float scale, int pose, float time, bool locked) const;
    void DrawAccessory(Vector2 body, Vector2 head, float scale) const;
    int bodyColor;
    int accessory;
    CharacterType type;
};
