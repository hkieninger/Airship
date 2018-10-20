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
    char name[16];
public:
    /*
     * make sure to call join or detach if you don't want a zombie thread
     * max length of @name is 16 chars inklusive \0
     */
    Thread(const char *name = NULL);

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