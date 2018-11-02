#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
#include "thread.h"

int Thread::count = 0;
pthread_mutex_t Thread::staticMutex = PTHREAD_MUTEX_INITIALIZER;

void *Thread::helperFunc(void *threadInstance) {
    Thread *thread = ((Thread *) threadInstance);
    if(thread->name[0]) {
        if(pthread_setname_np(thread->thread, thread->name) != 0)
            throw std::runtime_error("failed to set thread name: " + std::string(strerror(errno)));
    }
    thread->alive = true;
    thread->run();
    thread->alive = false;
    return NULL;
}

Thread::~Thread() {}

Thread::Thread(const char *name) : thread(-1), detached(false), alive(false) {
    if(name != NULL) {
        int length = strlen(name) + 1;
        if(length > 16)
            throw std::runtime_error("thread name exceeds maximal length of 16 chars");
        memcpy(this->name, name, length);
    } else {
        memset(this->name, 0, 16);
    }
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