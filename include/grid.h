#pragma once

#include "position.h"
#include <vector>

// Encapsula la matriz 20x10 y todas las operaciones sobre filas bloqueadas.
class Grid
{
public:
    Grid();
    void Initialize();
    void Draw(int offsetX, int offsetY, int cellSize) const;
    bool IsCellOutside(int row, int column) const;
    bool IsCellEmpty(int row, int column) const;
    int ClearFullRows();
    void LockCells(const std::vector<Position> &tiles, int blockId);

private:
    bool IsRowFull(int row) const;
    void ClearRow(int row);
    void MoveRowDown(int row, int numberOfRows);

    static const int numberOfRows = 20;
    static const int numberOfColumns = 10;
    int grid[numberOfRows][numberOfColumns];
};
