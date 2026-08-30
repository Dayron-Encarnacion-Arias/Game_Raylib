#include "colors.h"

const Color darkRed = {93, 7, 17, 255};
const Color mainRed = {225, 31, 55, 255};
const Color lightRed = {255, 91, 107, 255};
const Color inkBlack = {11, 9, 13, 255};
const Color panelBlack = {24, 18, 24, 245};
const Color mutedText = {150, 133, 142, 255};

// Construye la paleta de las piezas manteniendo un buen contraste.
std::vector<Color> GetCellColors()
{
    return {{74, 218, 255, 255}, {255, 220, 64, 255},
            {184, 86, 255, 255}, {69, 219, 103, 255},
            {255, 71, 80, 255}, {78, 105, 255, 255},
            {255, 148, 51, 255}};
}

// Construye la paleta seleccionable del avatar.
std::vector<Color> GetCharacterColors()
{
    return {{225, 35, 52, 255}, {241, 86, 38, 255},
            {184, 35, 77, 255}, {99, 40, 55, 255},
            {242, 181, 188, 255}};
}
