#include "subject.h"
#include "observer.h"
#include <vector>
#include <memory>

void Subject::attach(std::shared_ptr<Observer> o) { observers.emplace_back(o); }

void Subject::detach(std::shared_ptr<Observer> o) {
  for (auto it = observers.begin(); it != observers.end(); ) {
    if (*it == o) it = observers.erase(it);
    else ++it;
  }
}

void Subject::notifyObservers() {
  for (auto &ob : observers) ob->notify();
}