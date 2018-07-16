#ifndef OBSERVABLE_H
#define OBSERVABLE_H 1

#include <vector>
#include "observer.h"

/*
 * provides with Observer in observer.h a similar pattern as the Observer-Observable pattern in Java
 */
class Observable {
protected:
    std::vector<Observer *> observers;
public:
    /*
     * adds the observer to the observer vector
     */
    void addObserver(Observer *observer);

    /*
     * deletes the first occurance of the observer from observer vector
     */
    void deleteObserver(Observer *observer);

    /*
     * deletes all observers from the observer vector
     */
    void deleteObservers();

    /*
     * notifies all the observers in the observer vector by calling their update method
     * @data: data to pass to the update method
     */
    void notifyObservers(void *data);
};

#endif /* OBSERVABLE_H */