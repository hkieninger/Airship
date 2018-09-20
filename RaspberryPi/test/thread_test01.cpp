#include <stdio.h>
#include "../thread/thread.h"

class WorkerThread: public Thread {
public:

    void run() {
        WorkerThread *work = NULL;
        if(getNumber() < 3) {
            work = new WorkerThread();
            work->start();
        }
        printf("Hello world! %d\n", getNumber());
        if(getNumber() < 3) {
            work->join();
            delete work;
        }
    }
};

int main(void) {
    printf("start\n");
    WorkerThread ts[3];
    for(int i = 0; i < 3; i++) {
        ts[i].start();
    }
    for(int i = 3; i >= 0; i--) {
        ts[i].join();
    }
    printf("stop\n");
    return 0;
}