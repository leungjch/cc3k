#include <iostream>
#include "textObserver.h"
#include <memory>

using namespace std;

TextObserver::TextObserver(shared_ptr<CC3K> subject, int width, int height) : subject{subject}, width{width}, height{height}
{
}

TextObserver::~TextObserver() {
}

void TextObserver::notify() {
  for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          cout << subject->getState(j,i);
      }
      cout << endl;
  }
  cout << subject->getGameStatus() << endl;
}
