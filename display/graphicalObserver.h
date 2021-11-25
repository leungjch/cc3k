#ifndef GRAPHICALOBSERVER
#define GRAPHICALOBSERVER

#include "observer.h"
#include "../cc3k.h"
#include "window.h"
#include <iostream>
#include <memory>

class CC3K;

class GraphicalObserver : public Observer
{
    std::shared_ptr<CC3K> subject;
    int width;
    int height;
    const int SCALE = 16;
    std::shared_ptr<Xwindow> theWindow;
public:
    GraphicalObserver(std::shared_ptr<CC3K> subject, int width, int height);
    void notify() override;
    ~GraphicalObserver();
};

#endif
