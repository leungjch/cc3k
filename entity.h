#ifndef ENTITY_H
#define ENTITY_H
#include "utils/color.h"
class Entity{
    int x;
    int y;
    char symbol;
    std::string color;
  public:
    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);
    std::string getColor();
    char getSymbol();
    Entity(int x, int y, char sym);
    Entity(char sym);
    Entity(char sym, std::string color);

    Entity();
};
#endif
