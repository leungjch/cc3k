#ifndef ENTITY_H
#define ENTITY_H
class Entity{
    int x;
    int y;
    char symbol;
  public:
    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);
    char getSymbol();
    Entity(char sym);
    Entity();
};
#endif
