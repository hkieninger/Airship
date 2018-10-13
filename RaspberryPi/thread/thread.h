#ifndef THREAD_H
#define THREAD_H 1

#include <pthread.h>

class Thread {
    static int count;
    static pthread_mutex_t staticMutex;

    int number;
    pthread_t thread;
    static void *helperFunc(void *threadInstance);
    bool detached, alive;
public:
    /*
     * make sure to call join or detach if you don't want a zombie thread
     */
    Thread();

    virtual ~Thread();

    void start();

    void join();

    void detach();

    bool isAlive() {
        return alive;
    }

    bool isDetached() {
        return detached;
    }

    int getNumber() {
        return number;
    }

    virtual void run() = 0;
};

#endif /* THREAD_H */