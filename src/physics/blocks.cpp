#include "blocks.h"

// Define las cuatro orientaciones del tetromino L y lo centra al aparecer.
LBlock::LBlock()
{
    id = 1;
    cells[0] = {Position(0,2), Position(1,0), Position(1,1), Position(1,2)};
    cells[1] = {Position(0,1), Position(1,1), Position(2,1), Position(2,2)};
    cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,0)};
    cells[3] = {Position(0,0), Position(0,1), Position(1,1), Position(2,1)};
    Move(-1, 3);
}

// Define las cuatro orientaciones del tetromino J.
JBlock::JBlock()
{
    id = 2;
    cells[0] = {Position(0,0), Position(1,0), Position(1,1), Position(1,2)};
    cells[1] = {Position(0,1), Position(0,2), Position(1,1), Position(2,1)};
    cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,2)};
    cells[3] = {Position(0,1), Position(1,1), Position(2,0), Position(2,1)};
    Move(-1, 3);
}

// Define las dos orientaciones necesarias para el tetromino I.
IBlock::IBlock()
{
    id = 3;
    cells[0] = {Position(1,0), Position(1,1), Position(1,2), Position(1,3)};
    cells[1] = {Position(0,2), Position(1,2), Position(2,2), Position(3,2)};
    Move(-1, 3);
}

// Define el tetromino O; una sola rotacion evita movimientos visuales innecesarios.
OBlock::OBlock()
{
    id = 4;
    cells[0] = {Position(0,0), Position(0,1), Position(1,0), Position(1,1)};
    Move(-1, 4);
}

// Define las dos orientaciones del tetromino S.
SBlock::SBlock()
{
    id = 5;
    cells[0] = {Position(0,1), Position(0,2), Position(1,0), Position(1,1)};
    cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,2)};
    Move(-1, 3);
}

// Define las cuatro orientaciones del tetromino T.
TBlock::TBlock()
{
    id = 6;
    cells[0] = {Position(0,1), Position(1,0), Position(1,1), Position(1,2)};
    cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,1)};
    cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,1)};
    cells[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,1)};
    Move(-1, 3);
}

// Define las dos orientaciones del tetromino Z.
ZBlock::ZBlock()
{
    id = 7;
    cells[0] = {Position(0,0), Position(0,1), Position(1,1), Position(1,2)};
    cells[1] = {Position(0,2), Position(1,1), Position(1,2), Position(2,1)};
    Move(-1, 3);
}
