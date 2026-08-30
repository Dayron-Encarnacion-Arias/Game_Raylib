#pragma once

// Representa una celda mediante fila y columna dentro del tablero.
class Position
{
public:
    Position(int row, int column);
    int row;
    int column;
};
