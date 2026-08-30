#include "level.h"
#include "colors.h"
#include <cmath>

// Copia todos los datos que definen el aspecto y reglas de un nivel.
Level::Level(int id, std::string name, std::string location, std::string reward,
             Color primary, Color secondary, int targetLines, float fallTime)
    : id(id), name(name), location(location), reward(reward), primary(primary),
      secondary(secondary), targetLines(targetLines), fallTime(fallTime) {}

// Dibuja un escenario diferente para cada id mediante primitivas de Raylib.
void Level::DrawMap(float time, int width, int height) const
{
    ClearBackground(inkBlack);
    DrawRectangleGradientV(0, 0, width, height, {8, 7, 10, 255}, secondary);
    if (id == 0)
        for (int i = 0; i < 12; i++)
            DrawRectangle(i * 120 - static_cast<int>(std::fmod(time * 24, 120.0f)), 80 + (i % 3) * 150, 82, 12, Fade(primary, 0.20f));
    if (id == 1)
        for (int i = 0; i < 9; i++)
            DrawCircle(80 + i * 155, 650 - static_cast<int>(std::fmod(time * (18 + i), 380.0f)), 20 + i % 3 * 8, Fade(primary, 0.15f));
    if (id == 2)
        for (int i = 0; i < 18; i++)
        {
            int h = 80 + (i * 47) % 220;
            DrawRectangle(i * 76, height - h, 55, h, {15, 10, 25, 255});
            DrawRectangle(i * 76 + 8, height - h + 15, 5, 14, Fade(primary, 0.5f));
        }
    if (id == 3)
        for (int radius = 90; radius < 470; radius += 70)
            DrawCircleLines(width / 2, height / 2, radius + std::sin(time * 2) * 7, Fade(primary, 0.18f));
    if (id == 4)
        for (int i = 0; i < 8; i++)
        {
            DrawRectangle(i * 180, 100, 90, 620, {17, 10, 20, 255});
            DrawCircle(i * 180 + 45, 100, 45, Fade(primary, 0.16f));
        }
    if (id == 5)
        for (int i = 0; i < 7; i++)
        {
            float x = std::fmod(i * 233.0f + time * 90, 1400.0f) - 60;
            DrawLineEx({x, 0}, {x - 220, 720}, 2, Fade(primary, 0.22f));
        }
    if (id == 6)
        for (int i = 0; i < 9; i++)
        {
            Vector2 c = {70.0f + i * 150, 110.0f + (i % 3) * 240};
            DrawCircleLines(c.x, c.y, 50, Fade(primary, 0.18f));
            for (int k = 0; k < 8; k++)
                DrawRectanglePro({c.x, c.y, 70, 8}, {0, 4}, k * 45.0f + time * 10, Fade(primary, 0.12f));
        }
    if (id == 7)
        for (int i = 0; i < 80; i++)
            DrawCircle((i * 97) % width, (i * 53 + static_cast<int>(time * (i % 3 + 1) * 3)) % height, i % 3 + 1, Fade(WHITE, 0.25f));
    if (id == 8)
        for (int i = 0; i < 14; i++)
            DrawTriangle({i * 100.0f, 720}, {i * 100.0f + 50, 480.0f + std::sin(time + i) * 35}, {i * 100.0f + 100, 720}, Fade(primary, 0.11f));
    if (id == 9)
        for (int i = 0; i < 12; i++)
        {
            float a = i * PI / 6 + time * 0.05f;
            DrawLineEx({width / 2.0f, height / 2.0f}, {width / 2.0f + std::cos(a) * 900, height / 2.0f + std::sin(a) * 900}, 22, Fade(primary, 0.08f));
        }
    DrawRectangleGradientH(0, 0, 260, height, Fade(BLACK, 0.72f), BLANK);
    DrawRectangleGradientH(width - 260, 0, 260, height, BLANK, Fade(BLACK, 0.72f));
}

// Devuelve el indice usado para seleccionar mapa y shader.
int Level::GetId() const { return id; }
// Devuelve el titulo visible de la fase.
const std::string &Level::GetName() const { return name; }
// Devuelve la descripcion corta del escenario.
const std::string &Level::GetLocation() const { return location; }
// Devuelve el nombre del accesorio entregado como premio.
const std::string &Level::GetReward() const { return reward; }
// Devuelve el color principal de la interfaz de la fase.
Color Level::GetPrimaryColor() const { return primary; }
// Devuelve el color utilizado en el fondo del mapa.
Color Level::GetSecondaryColor() const { return secondary; }
// Devuelve la cantidad de lineas necesaria para ganar.
int Level::GetTargetLines() const { return targetLines; }
// Devuelve el intervalo base entre descensos automaticos.
float Level::GetFallTime() const { return fallTime; }

// Define los diez niveles originales, sus metas y recompensas.
std::vector<Level> GetAllLevels()
{
    return {
        Level(0, "01 / SUBSUELO", "Tuneles de carbon", "Visor rojo", {205, 30, 45, 255}, {66, 12, 23, 255}, 8, 0.72f),
        Level(1, "02 / FUNDICION", "Rios de metal", "Gorra rebelde", {255, 73, 35, 255}, {91, 17, 7, 255}, 10, 0.66f),
        Level(2, "03 / NEON", "Distrito nocturno", "Audifonos", {255, 28, 114, 255}, {41, 8, 63, 255}, 12, 0.60f),
        Level(3, "04 / REACTOR", "Nucleo inestable", "Antena", {237, 43, 43, 255}, {78, 27, 10, 255}, 14, 0.54f),
        Level(4, "05 / CATEDRAL", "Vidrio y ceniza", "Corona", {173, 21, 46, 255}, {38, 16, 47, 255}, 16, 0.49f),
        Level(5, "06 / TORMENTA", "Cielo electrico", "Capa", {222, 36, 74, 255}, {25, 28, 61, 255}, 18, 0.44f),
        Level(6, "07 / FABRICA", "Engranajes vivos", "Llave mecanica", {240, 48, 43, 255}, {55, 30, 20, 255}, 20, 0.39f),
        Level(7, "08 / VACIO", "Orbita quebrada", "Halo", {227, 42, 78, 255}, {25, 12, 45, 255}, 22, 0.35f),
        Level(8, "09 / ABISMO", "Ecos bajo cero", "Cuernos", {180, 19, 45, 255}, {9, 24, 44, 255}, 24, 0.31f),
        Level(9, "10 / CORONA", "Trono del bloque", "Mascara legendaria", {255, 42, 54, 255}, {75, 5, 14, 255}, 28, 0.27f)};
}
