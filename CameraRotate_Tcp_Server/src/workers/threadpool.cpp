#include "threadpool.h"

/*********************************************************************
 *                                                                   *
 *                       Thread Pool Functions                       *
 *                                                                   *
 *********************************************************************/

/* Thread Pool Constructor */
ThreadPool::ThreadPool(int pool_size) {

    worker_threads.clear();

    for (int i = 0; i < pool_size; i++) {
        pthread_t* worker = (pthread_t*)malloc(sizeof(pthread_t));
        worker_threads.push_back(worker);
    }
}

/* Thread Pool Destructor */
ThreadPool::~ThreadPool() {

    for (int i = 0; i < worker_threads.size(); i++) {
        free(worker_threads[i]);
    }
    worker_threads.clear();
}

/* Create Thread Pool */
void ThreadPool::Create(TaskQueue* taskQueue) {

    for (int i = 0; i < worker_threads.size(); i++) {
        if (pthread_create(worker_threads[i], NULL, TaskQueue::StartThread, (void*)taskQueue) != 0) {
            std::cout << "pthread_create error..." << std::endl;
            exit(0);
        }
        sleep(1);
    }
}

void ThreadPool::addWorker(TaskQueue* taskQueue) {

    pthread_t* worker = (pthread_t*)malloc(sizeof(pthread_t));
    worker_threads.push_back(worker);
    if (pthread_create(worker_threads[worker_threads.size()-1], NULL, TaskQueue::StartThread, (void*)taskQueue) != 0) {
        std::cout << "pthread_create error..." << std::endl;
        exit(0);
    }
}

void ThreadPool::JoinWorker() {
        if (pthread_join(*worker_threads[worker_threads.size()-1], NULL) != 0) {
            std::cout << "pthread_join error..." << std::endl;
            exit(0);
        }
}


/* Join Thread */
void ThreadPool::Join() {

    for (int i = 0; i < worker_threads.size(); i++) {
        if (pthread_join(*worker_threads[i], NULL) != 0) {
            std::cout << "pthread_join error..." << std::endl;
            exit(0);
        }
    }
}

