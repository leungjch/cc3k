#include <iostream>
#include "graphicalObserver.h"
#include "window.h"
#include "../cc3k.h"
#include <memory>

using namespace std;

GraphicalObserver::GraphicalObserver(CC3K* subject, int width, int height)
    : subject{subject}, width{width}, height{height}
{
  // Launch a new XWindow
  theWindow = make_shared<Xwindow>(SCALE * (width), SCALE * (height));
}

GraphicalObserver::~GraphicalObserver()
{

}

// Returns true if the char ch is an upper case char
bool isUpper(char ch)
{
  return (ch >= 'A' && ch <= 'Z');
}

// Returns true if the char ch is an lower case char
bool isLower(char ch)
{
  return (ch >= 'a' && ch <= 'z');
}

// Returns true if the char ch is a digit
bool isDigit(char ch)
{
  return (ch >= '0' && ch <= '9');
}

void GraphicalObserver::notify()
{
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      char val = subject->getState(j, i).first;
      unsigned long col = Xwindow::White;

      if (val == Cell::WALL_HORIZONTAL || val == Cell::WALL_VERTICAL)
      {
        theWindow->drawImage(theWindow->wallImg, j*SCALE, i*SCALE);

      }
      else if (val == '@')
      {
        // Draw the floor

        theWindow->drawImage(theWindow->pcImg, j*SCALE, i*SCALE);

      }
      else if (val == Cell::PASSAGE) 
      {
        col = Xwindow::Green;
        theWindow->drawImage(theWindow->passageImg, j*SCALE, i*SCALE);
      }
      else if (val == Cell::TILE)
      {
        theWindow->drawImage(theWindow->floorImg, j*SCALE, i*SCALE);
      }
      else if (val == Cell::DOOR) 
      {
        col = Xwindow::Green;
        theWindow->drawImage(theWindow->doorImg, j*SCALE, i*SCALE);
      }
      else if (val == 'P') 
      {
        theWindow->drawImage(theWindow->potionImg, j*SCALE, i*SCALE);
      }
      else if (val == 'G') 
      {
        theWindow->drawImage(theWindow->goldImg, j*SCALE, i*SCALE);
      }
      else if (val == '\\')
      {
        theWindow->drawImage(theWindow->stairwayImg, j*SCALE, i*SCALE);
      }

      else if (val == ' ')
      {
        theWindow->drawImage(theWindow->spaceImg, j*SCALE, i*SCALE);

        // col = Xwindow::Black;
        // theWindow->fillRectangle(j * SCALE, i * SCALE, SCALE, SCALE, col);
      }
      else {
        // theWindow->fillRectangle(j * SCALE, i * SCALE, SCALE, SCALE, col);
        theWindow->drawImage(theWindow->pcImg, j*SCALE, i*SCALE);
      }
    }
  }
}
