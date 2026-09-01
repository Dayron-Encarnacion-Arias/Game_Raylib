#include "character.h"
#include "colors.h"
#include <algorithm>
#include <cmath>

// Crea el personaje con el primer color y sin accesorio.
Character::Character()
{
    bodyColor = 0;
    accessory = 0;
    type = CharacterType::Operator;
}

// Aplica la animacion comun y delega el dibujo al tipo de personaje equipado.
void Character::Draw(Vector2 body, float scale, int pose, float time, bool locked) const
{
    body.y += std::sin(time * 3.0f + pose) * 2.0f * scale;
    if (type == CharacterType::CrimsonCadet)
        DrawCrimsonCadet(body, scale, pose, time, locked);
    else
        DrawOperator(body, scale, pose, time, locked);
}

// Dibuja el operador base y el accesorio normal que tenga equipado.
void Character::DrawOperator(Vector2 body, float scale, int pose, float time, bool locked) const
{
    const std::vector<Color> colors = GetCharacterColors();
    const Color color = locked ? Color{67, 60, 64, 255} : colors[bodyColor];
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

// Dibuja al Cadete Carmesi con uniforme, gorra, mascara, mochila y prisma permanentes.
void Character::DrawCrimsonCadet(Vector2 body, float z, int pose, float time, bool locked) const
{
    const Color navy = locked ? Color{54, 55, 60, 255} : Color{20, 42, 61, 255};
    const Color gold = locked ? Color{91, 88, 82, 255} : Color{238, 184, 56, 255};
    const Color mask = locked ? Color{72, 65, 67, 255} : Color{151, 38, 43, 255};
    const Color hair = locked ? Color{88, 84, 75, 255} : Color{224, 164, 58, 255};
    const Color skin = locked ? Color{90, 83, 81, 255} : Color{242, 190, 151, 255};
    const Color brown = locked ? Color{63, 57, 57, 255} : Color{91, 47, 38, 255};
    const Vector2 head = {body.x, body.y - 27.0f * z};
    const float arm = pose % 3 == 0 ? -0.75f : pose % 3 == 1 ? 0.35f
                                                             : std::sin(time * 4.0f);
    const float leg = pose % 2 == 0 ? -0.3f : 0.3f;

    DrawRectangleRounded({body.x + 7 * z, body.y - 8 * z, 15 * z, 31 * z}, 0.35f, 5, brown);
    DrawLineEx({body.x - 7 * z, body.y + 2 * z},
               {body.x - (17 + arm * 7) * z, body.y + (12 + arm * 11) * z}, 7 * z, navy);
    DrawCircleV({body.x - (18 + arm * 7) * z, body.y + (13 + arm * 11) * z}, 3 * z, skin);
    DrawLineEx({body.x + 7 * z, body.y + 2 * z},
               {body.x + (17 - arm * 7) * z, body.y + (12 - arm * 11) * z}, 7 * z, navy);
    DrawCircleV({body.x + (18 - arm * 7) * z, body.y + (13 - arm * 11) * z}, 3 * z, skin);

    DrawRectangleRounded({body.x - 11 * z, body.y - 8 * z, 22 * z, 29 * z}, 0.3f, 5, navy);
    DrawRectangleRoundedLinesEx({body.x - 11 * z, body.y - 8 * z, 22 * z, 29 * z}, 0.3f, 5, z, gold);
    DrawTriangle({body.x - 6 * z, body.y - 7 * z}, {body.x + 6 * z, body.y - 7 * z},
                 {body.x, body.y + 5 * z}, gold);

    DrawLineEx({body.x - 5 * z, body.y + 20 * z},
               {body.x - (8 + leg * 5) * z, body.y + 39 * z}, 7 * z, skin);
    DrawLineEx({body.x + 5 * z, body.y + 20 * z},
               {body.x + (8 - leg * 5) * z, body.y + 39 * z}, 7 * z, skin);
    DrawRectangleV({body.x - (12 + leg * 5) * z, body.y + 25 * z}, {9 * z, 9 * z}, navy);
    DrawRectangleV({body.x + (3 - leg * 5) * z, body.y + 25 * z}, {9 * z, 9 * z}, navy);
    DrawLineEx({body.x - (8 + leg * 5) * z, body.y + 34 * z},
               {body.x - (8 + leg * 5) * z, body.y + 40 * z}, 6 * z, WHITE);
    DrawLineEx({body.x + (8 - leg * 5) * z, body.y + 34 * z},
               {body.x + (8 - leg * 5) * z, body.y + 40 * z}, 6 * z, WHITE);
    DrawRectangleRounded({body.x - (14 + leg * 5) * z, body.y + 38 * z, 12 * z, 5 * z}, 0.4f, 4, brown);
    DrawRectangleRounded({body.x + (2 - leg * 5) * z, body.y + 38 * z, 12 * z, 5 * z}, 0.4f, 4, brown);

    DrawCircleV({head.x - 12 * z, head.y - 3 * z}, 6 * z, hair);
    DrawCircleV({head.x + 12 * z, head.y - 2 * z}, 6 * z, hair);
    DrawCircleV(head, 15 * z, mask);
    DrawCircleV({head.x - 6 * z, head.y - 3 * z}, 5 * z, WHITE);
    DrawCircleV({head.x + 6 * z, head.y - 3 * z}, 5 * z, WHITE);
    DrawCircleV({head.x - 5 * z, head.y - 3 * z}, 2 * z, locked ? DARKGRAY : SKYBLUE);
    DrawCircleV({head.x + 5 * z, head.y - 3 * z}, 2 * z, locked ? DARKGRAY : SKYBLUE);
    DrawCircleV({head.x - 7 * z, head.y + 9 * z}, 5 * z, brown);
    DrawCircleV({head.x + 7 * z, head.y + 9 * z}, 5 * z, brown);
    DrawCircleV({head.x - 7 * z, head.y + 9 * z}, 2.5f * z, BLACK);
    DrawCircleV({head.x + 7 * z, head.y + 9 * z}, 2.5f * z, BLACK);

    DrawRectangleRounded({head.x - 15 * z, head.y - 22 * z, 30 * z, 11 * z}, 0.65f, 6, WHITE);
    DrawRectangleRounded({head.x + 4 * z, head.y - 22 * z, 11 * z, 11 * z}, 0.55f, 5, mask);
    DrawRectangleRounded({head.x - 18 * z, head.y - 14 * z, 26 * z, 4 * z}, 0.5f, 4, mask);
    DrawLineEx({head.x - 4 * z, head.y - 18 * z}, {head.x, head.y - 21 * z}, 2 * z, locked ? DARKGRAY : DARKGREEN);
    DrawLineEx({head.x, head.y - 21 * z}, {head.x + 5 * z, head.y - 18 * z}, 2 * z, locked ? DARKGRAY : DARKGREEN);

    const Vector2 prism = {body.x - (27 + arm * 5) * z, body.y - (4 - arm * 5) * z};
    DrawPoly(prism, 6, 6 * z, 30.0f, locked ? GRAY : SKYBLUE);
    DrawPolyLinesEx(prism, 6, 6 * z, 30.0f, z, gold);
}

// Limita y asigna el indice del color seleccionado.
void Character::SetBodyColor(int colorIndex)
{
    bodyColor = std::max(0, std::min(4, colorIndex));
}

// Limita y asigna el accesorio que se mostrara sobre el personaje.
void Character::SetAccessory(int accessoryIndex)
{
    if (type != CharacterType::Operator)
        return;
    accessory = std::max(0, std::min(10, accessoryIndex));
}

// Selecciona el operador base o uno de los personajes exclusivos disponibles.
void Character::SetType(int typeIndex)
{
    type = typeIndex == 1 ? CharacterType::CrimsonCadet : CharacterType::Operator;
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

// Devuelve el identificador serializable del tipo de personaje equipado.
int Character::GetType() const
{
    return static_cast<int>(type);
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
