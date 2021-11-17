#ifndef CELL_H
#define CELL_H
#include "entity.h"
class Cell {
    char ch;
    int chamberNum;

    // Constants for cell types
    static const char WALL_HORIZONTAL = '|';
    static const char WALL_VERTICAL = '-';
    static const char TILE = '.';
    static const char PASSAGE = '#';
    static const char DOOR = '+';
    static const char NOTHING = ' ';

    public:
        Cell();
        Cell(char c);
        char getChar();
        int getChamberNum();


};
#endif
