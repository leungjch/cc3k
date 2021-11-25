#ifndef TEXTOBSERVER_H
#define TEXTOBSERVER_H

#include "observer.h"
#include "../cc3k.h"
#include <iostream>
#include <memory>

class CC3K;

class TextObserver : public Observer
{
    std::shared_ptr<CC3K> subject;
    int width;
    int height;
    std::ostream &out = std::cout;

public:
    TextObserver(std::shared_ptr<CC3K> subject, int width, int height);
    void notify() override;
    ~TextObserver();
};

#endif
