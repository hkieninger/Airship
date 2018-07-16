#include <vector>
#include "observer.h"
#include "observable.h"

void Observable::addObserver(Observer *observer) {
    observers.push_back(observer);
}

void Observable::deleteObserver(Observer *observer) {
    for(std::vector<Observer *>::iterator it = observers.begin(); it != observers.end(); ++it) {
        if(observer == *it) {
            observers.erase(it);
            break;
        }
    }
}

void Observable::deleteObservers() {
    observers.clear();
}

void Observable::notifyObservers(void *data) {
    for(std::vector<Observer *>::iterator it = observers.begin(); it != observers.end(); ++it) {
        (*it)->update(/*this, */data);
    }
}