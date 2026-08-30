#pragma once

#include "position.h"
#include <raylib.h>
#include <map>
#include <vector>

// Clase base que almacena las rotaciones y la posicion de cualquier tetromino.
class Block
{
public:
    Block();
    void Draw(int offsetX, int offsetY, int cellSize = 28) const;
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions() const;
    void Rotate();
    void UndoRotation();
    int id;

protected:
    std::map<int, std::vector<Position>> cells;

private:
    int cellSize;
    int rotationState;
    int rowOffset;
    int columnOffset;
    std::vector<Color> colors;
};
