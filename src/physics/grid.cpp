#include "grid.h"
#include "colors.h"
#include <raylib.h>

// Crea un tablero listo para recibir piezas.
Grid::Grid()
{
    Initialize();
}

// Coloca cero en cada celda para representar un tablero vacio.
void Grid::Initialize()
{
    for (int row = 0; row < numberOfRows; row++)
        for (int column = 0; column < numberOfColumns; column++)
            grid[row][column] = 0;
}

// Dibuja la cuadricula y los bloques que ya fueron fijados.
void Grid::Draw(int offsetX, int offsetY, int cellSize) const
{
    const std::vector<Color> colors = GetCellColors();
    for (int row = 0; row < numberOfRows; row++)
    {
        for (int column = 0; column < numberOfColumns; column++)
        {
            Rectangle cell = {static_cast<float>(offsetX + column * cellSize),
                              static_cast<float>(offsetY + row * cellSize),
                              static_cast<float>(cellSize), static_cast<float>(cellSize)};
            DrawRectangleLinesEx(cell, 1, {38, 31, 37, 150});
            const int value = grid[row][column];
            if (value != 0)
            {
                Rectangle block = {cell.x + 2, cell.y + 2, cell.width - 4, cell.height - 4};
                DrawRectangleRounded(block, 0.18f, 4, colors[value - 1]);
                DrawRectangle(block.x + 4, block.y + 4,
                              block.width - 8, 3, Fade(WHITE, 0.25f));
            }
        }
    }
}

// Indica si una coordenada se encuentra fuera de los limites del tablero.
bool Grid::IsCellOutside(int row, int column) const
{
    return row < 0 || row >= numberOfRows || column < 0 || column >= numberOfColumns;
}

// Comprueba si una celda valida todavia no contiene un bloque fijo.
bool Grid::IsCellEmpty(int row, int column) const
{
    if (row < 0) return true;
    return !IsCellOutside(row, column) && grid[row][column] == 0;
}

// Copia las cuatro celdas de una pieza a la matriz permanente.
void Grid::LockCells(const std::vector<Position> &tiles, int blockId)
{
    for (const Position &tile : tiles)
        if (!IsCellOutside(tile.row, tile.column)) grid[tile.row][tile.column] = blockId;
}

// Elimina filas llenas, baja las superiores y devuelve cuantas fueron limpiadas.
int Grid::ClearFullRows()
{
    int completed = 0;
    for (int row = numberOfRows - 1; row >= 0; row--)
    {
        if (IsRowFull(row))
        {
            ClearRow(row);
            completed++;
        }
        else if (completed > 0)
        {
            MoveRowDown(row, completed);
        }
    }
    return completed;
}

// Revisa si todas las columnas de una fila contienen bloques.
bool Grid::IsRowFull(int row) const
{
    for (int column = 0; column < numberOfColumns; column++)
        if (grid[row][column] == 0) return false;
    return true;
}

// Convierte una fila completa nuevamente en celdas vacias.
void Grid::ClearRow(int row)
{
    for (int column = 0; column < numberOfColumns; column++) grid[row][column] = 0;
}

// Mueve una fila hacia abajo despues de limpiar una o mas lineas.
void Grid::MoveRowDown(int row, int numberOfRowsToMove)
{
    for (int column = 0; column < numberOfColumns; column++)
    {
        grid[row + numberOfRowsToMove][column] = grid[row][column];
        grid[row][column] = 0;
    }
}
