#include "block.h"
#include "colors.h"
#include <raylib.h>

// Prepara un bloque en su primera rotacion y en el origen de su matriz local.
Block::Block()
{
    cellSize = 28;
    rotationState = 0;
    rowOffset = 0;
    columnOffset = 0;
    colors = GetCellColors();
}

// Dibuja las cuatro celdas del bloque con el desplazamiento solicitado.
void Block::Draw(int offsetX, int offsetY, int drawCellSize) const
{
    const std::vector<Position> tiles = GetCellPositions();
    for (const Position &tile : tiles)
    {
        Rectangle rectangle = {
            static_cast<float>(offsetX + tile.column * drawCellSize + 2),
            static_cast<float>(offsetY + tile.row * drawCellSize + 2),
            static_cast<float>(drawCellSize - 4),
            static_cast<float>(drawCellSize - 4)};
        DrawRectangleRounded(rectangle, 0.18f, 4, colors[id - 1]);
        DrawRectangle(rectangle.x + 4, rectangle.y + 4,
                      rectangle.width - 8, 3, Fade(WHITE, 0.28f));
    }
}

// Desplaza el bloque una cantidad de filas y columnas.
void Block::Move(int rows, int columns)
{
    rowOffset += rows;
    columnOffset += columns;
}

// Convierte las celdas locales de la rotacion actual a coordenadas del tablero.
std::vector<Position> Block::GetCellPositions() const
{
    std::vector<Position> movedTiles;
    const std::vector<Position> &tiles = cells.at(rotationState);
    for (const Position &tile : tiles)
        movedTiles.push_back(Position(tile.row + rowOffset, tile.column + columnOffset));
    return movedTiles;
}

// Avanza circularmente a la siguiente rotacion definida por la clase derivada.
void Block::Rotate()
{
    rotationState++;
    if (rotationState == static_cast<int>(cells.size())) rotationState = 0;
}

// Regresa una rotacion cuando Game determina que la nueva posicion no es valida.
void Block::UndoRotation()
{
    rotationState--;
    if (rotationState == -1) rotationState = static_cast<int>(cells.size()) - 1;
}
