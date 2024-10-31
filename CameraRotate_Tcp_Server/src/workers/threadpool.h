#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <list>

#include "taskqueue.h"

class ThreadPool
{
    public:
        ThreadPool(int pool_size);
        ~ThreadPool();
        void Create(TaskQueue* taskQueue);
        void addWorker(TaskQueue* taskQueue);
        void Join();
        void JoinWorker();
    private:
        std::vector<pthread_t*> worker_threads;
};


#endif // THREADPOOL_H
