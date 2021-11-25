#include <iostream>
#include "graphicalObserver.h"
#include "window.h"
#include "../cc3k.h"
#include <memory>

using namespace std;

GraphicalObserver::GraphicalObserver(shared_ptr<CC3K> subject, int width, int height)
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
      char val = subject->getState(j, i);
      unsigned long col = Xwindow::White;
      if (val == '.')
      {
        col = Xwindow::Green;
      }
      else if (isLower(val))
      {
        col = Xwindow::Red;
      }
      else if (val == '@')
      {
        col = Xwindow::Blue;
      }
      else if (val == '#') 
      {
        col = Xwindow::Green;
      }
      else if (val == ' ')
      {
        col = Xwindow::White;
      }
      else
      {
        col = Xwindow::Black;
      }
      // Render the square
      theWindow->fillRectangle(j * SCALE, i * SCALE, SCALE, SCALE, col);
      if (col == Xwindow::Black) {
        theWindow->drawImage(theWindow->theImg, j*SCALE, i*SCALE);

      }
    }
  }
}
