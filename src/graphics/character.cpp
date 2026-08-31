#include "character.h"
#include "colors.h"
#include <algorithm>
#include <cmath>

// Crea el personaje con el primer color y sin accesorio.
Character::Character()
{
    bodyColor = 0;
    accessory = 0;
}

// Dibuja cuerpo, extremidades, rostro y accesorio usando primitivas de Raylib.
void Character::Draw(Vector2 body, float scale, int pose, float time, bool locked) const
{
    const std::vector<Color> colors = GetCharacterColors();
    const Color color = locked ? Color{67, 60, 64, 255} : colors[bodyColor];
    body.y += std::sin(time * 3.0f + pose) * 2.0f * scale;
    const Vector2 head = {body.x, body.y - 26.0f * scale};
    const float arm = pose % 3 == 0 ? -0.7f : pose % 3 == 1 ? 0.45f
                                                            : std::sin(time * 4.0f);
    const float leg = pose % 2 == 0 ? -0.35f : 0.35f;

    if (accessory == 6 && !locked)
        DrawTriangle({body.x - 8 * scale, body.y - 3 * scale}, {body.x - 18 * scale, body.y + 31 * scale},
                     {body.x + 11 * scale, body.y + 24 * scale}, darkRed);

    DrawLineEx({body.x - 7 * scale, body.y + 3 * scale},
               {body.x - (16 + arm * 7) * scale, body.y + (13 + arm * 12) * scale}, 6 * scale, color);
    DrawLineEx({body.x + 7 * scale, body.y + 3 * scale},
               {body.x + (16 - arm * 7) * scale, body.y + (13 - arm * 12) * scale}, 6 * scale, color);
    DrawRectangleRounded({body.x - 10 * scale, body.y - 7 * scale, 20 * scale, 29 * scale}, 0.35f, 6, color);
    DrawLineEx({body.x - 5 * scale, body.y + 20 * scale},
               {body.x - (8 + leg * 6) * scale, body.y + 38 * scale}, 7 * scale, color);
    DrawLineEx({body.x + 5 * scale, body.y + 20 * scale},
               {body.x + (8 - leg * 6) * scale, body.y + 38 * scale}, 7 * scale, color);
    DrawCircleV(head, 14 * scale, color);
    DrawCircleV({head.x - 5 * scale, head.y - scale}, 2 * scale, locked ? DARKGRAY : WHITE);
    DrawCircleV({head.x + 5 * scale, head.y - scale}, 2 * scale, locked ? DARKGRAY : WHITE);
    if (!locked)
        DrawAccessory(body, head, scale);
}

// Limita y asigna el indice del color seleccionado.
void Character::SetBodyColor(int colorIndex)
{
    bodyColor = std::max(0, std::min(4, colorIndex));
}

// Limita y asigna el accesorio que se mostrara sobre el personaje.
void Character::SetAccessory(int accessoryIndex)
{
    accessory = std::max(0, std::min(10, accessoryIndex));
}

// Devuelve el color actual para guardarlo o marcarlo en la interfaz.
int Character::GetBodyColor() const
{
    return bodyColor;
}

// Devuelve el accesorio equipado actualmente.
int Character::GetAccessory() const
{
    return accessory;
}

// Dibuja uno de los diez premios desbloqueables segun su identificador.
void Character::DrawAccessory(Vector2 body, Vector2 head, float z) const
{
    if (accessory == 1)
        DrawRectangleRounded({head.x - 13 * z, head.y - 4 * z, 26 * z, 7 * z}, 0.5f, 4, lightRed);
    if (accessory == 2)
    {
        DrawRectangleV({head.x - 13 * z, head.y - 15 * z}, {23 * z, 6 * z}, lightRed);
        DrawRectangleV({head.x + 5 * z, head.y - 10 * z}, {14 * z, 4 * z}, lightRed);
    }
    if (accessory == 3)
    {
        DrawCircleLines(head.x, head.y, 17 * z, lightRed);
        DrawRectangleV({head.x - 19 * z, head.y - 4 * z}, {5 * z, 12 * z}, lightRed);
        DrawRectangleV({head.x + 14 * z, head.y - 4 * z}, {5 * z, 12 * z}, lightRed);
    }
    if (accessory == 4)
    {
        DrawLineEx({head.x, head.y - 13 * z}, {head.x, head.y - 27 * z}, 3 * z, lightRed);
        DrawCircleV({head.x, head.y - 29 * z}, 4 * z, lightRed);
    }
    if (accessory == 5)
    {
        DrawRectangleRounded({head.x - 16 * z, head.y - 15 * z, 32 * z, 6 * z}, 0.35f, 4, GOLD);
        for (int i = -1; i <= 1; i++)
        {
            const Vector2 left = {head.x + (i * 10 - 6) * z, head.y - 14 * z};
            const Vector2 right = {head.x + (i * 10 + 6) * z, head.y - 14 * z};
            const Vector2 tip = {head.x + i * 10 * z, head.y - (i == 0 ? 30 : 24) * z};
            // Raylib requiere los vertices de DrawTriangle en orden antihorario.
            DrawTriangle(left, right, tip, GOLD);
        }
    }
    if (accessory == 7)
    {
        DrawLineEx({body.x + 16 * z, body.y + 11 * z}, {body.x + 29 * z, body.y - 5 * z}, 4 * z, LIGHTGRAY);
        DrawCircleLines(body.x + 31 * z, body.y - 8 * z, 6 * z, LIGHTGRAY);
    }
    if (accessory == 8)
        DrawEllipseLines(head.x, head.y - 22 * z, 18 * z, 5 * z, GOLD);
    if (accessory == 9)
    {
        DrawTriangle({head.x - 12 * z, head.y - 10 * z}, {head.x - 4 * z, head.y - 14 * z}, {head.x - 20 * z, head.y - 26 * z}, lightRed);
        DrawTriangle({head.x + 12 * z, head.y - 10 * z}, {head.x + 20 * z, head.y - 26 * z}, {head.x + 4 * z, head.y - 14 * z}, lightRed);
    }
    if (accessory == 10)
    {
        DrawRectangleRounded({head.x - 12 * z, head.y - 8 * z, 24 * z, 17 * z}, 0.35f, 5, {35, 25, 29, 255});
        DrawLineEx({head.x - 7 * z, head.y}, {head.x + 7 * z, head.y}, 2 * z, GOLD);
    }
}
