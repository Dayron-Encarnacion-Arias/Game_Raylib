#pragma once

#include <raylib.h>
#include <vector>

extern const Color darkRed;
extern const Color mainRed;
extern const Color lightRed;
extern const Color inkBlack;
extern const Color panelBlack;
extern const Color mutedText;

// Devuelve los siete colores utilizados por las piezas del Tetris.
std::vector<Color> GetCellColors();

// Devuelve los cinco colores disponibles para el personaje.
std::vector<Color> GetCharacterColors();
