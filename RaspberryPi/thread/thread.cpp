#include <pthread.h>
#include <stdio.h>
#include "thread.h"

int Thread::count = 0;
pthread_mutex_t Thread::staticMutex = PTHREAD_MUTEX_INITIALIZER;

void *Thread::helperFunc(void *threadInstance) {
    Thread *thread = ((Thread *) threadInstance);
    thread->alive = true;
    thread->run();
    thread->alive = false;
    return NULL;
}

Thread::Thread() : thread(-1), detached(false), alive(false) {
    pthread_mutex_lock(&staticMutex);
    number = count;
    count++;
    pthread_mutex_unlock(&staticMutex);
}

void Thread::start() {
    pthread_create(&thread, NULL, &Thread::helperFunc, this);
}

void Thread::join() {
    pthread_join(thread, NULL);
}

void Thread::detach() {
    pthread_detach(thread);
    detached = true;
}