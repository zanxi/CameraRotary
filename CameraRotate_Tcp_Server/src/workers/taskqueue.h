#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <list>

#include "sessionclient.h"

#define MAX_QUEUE_SIZE	20
#define MAX_POOL_SIZE	10


class TaskQueue
{
    public:
        TaskQueue(int queue_size);
        ~TaskQueue();
        void PopTask();
        void PopTaskMessage();
        void PopTaskSessionClient();
        void PushTask(int i);
        void PushTaskMessage(std::string msg);
        void PushTaskSessionClient(SessionClient sc);
        static void* StartThread(void* object); // function executed by worker threads
        void GetPacketData(); // function called by StartThread function
        void ListeningTaskQueue();        
        void Lock(std::string msg);
        void Unlock(std::string msg);
        void Wait(std::string msg);
        void Signal(std::string msg);
    private:
        std::list<int> task_queue;
        std::list<std::string> task_queue_message_client;
        std::list<SessionClient> task_queue_SessionClient;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
};



/* SIGINT Handler */
void SignalHandler(int sig_value);


#endif // TASKQUEUE_H
