#include "position.h"

// Inicializa una posicion inmutable durante cada calculo de bloque.
Position::Position(int row, int column)
{
    this->row = row;
    this->column = column;
}
