#ifndef OBSERVER_H
#define OBSERVER_H 1

//#include "observable.h"

/*
 * provides with Observable in observable.h a similar pattern as the Observer-Observable pattern in Java
 */
class Observer {
    public:
        virtual void update(/*Observable *observable, */void *data) = 0;
};

#endif /* OBSERVER_H */