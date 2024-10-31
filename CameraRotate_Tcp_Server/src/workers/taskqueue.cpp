#include "taskqueue.h"

#include <map>

/*********************************************************************
 *                                                                   *
 *                       Task Queue Functions                        *
 *                                                                   *
 *********************************************************************/

bool SIGINT_FLAG = false;

void SignalHandler(int sig_value) {
    SIGINT_FLAG = true;
}

/* Task Queue Constructor */
TaskQueue::TaskQueue(int queue_size) {

    task_queue.clear();
    task_queue_message_client.clear();
    task_queue_SessionClient.clear();
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < queue_size; i++) {
        task_queue.push_back(i);
    }
}

/* Task Queue Destructor */
TaskQueue::~TaskQueue() {

    task_queue.clear();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

/* Pop Task */
void TaskQueue::PopTask() {

    std::cout << "task_queue.front(): " << task_queue.front() << std::endl;
    task_queue.pop_front();
}

void TaskQueue::PopTaskMessage() {

    //std::cout << "task_queue.front(): " << task_queue.front() << std::endl;
    task_queue_message_client.pop_front();
}

void TaskQueue::PopTaskSessionClient() {

    //std::cout << "task_queue.front(): " << task_queue.front() << std::endl;
    task_queue_SessionClient.pop_front();
}

void TaskQueue::PushTask(int i) {
    task_queue.push_back(i);
}

void TaskQueue::PushTaskSessionClient(SessionClient sc) {
    task_queue_SessionClient.push_back(sc);
}

void TaskQueue::PushTaskMessage(std::string msg) {
    task_queue_message_client.push_back(msg);
}



/* static Start Thread */
void* TaskQueue::StartThread(void* object) {

    TaskQueue* taskQueue = (TaskQueue*)object;
    taskQueue->GetPacketData();
}

/* Get Packet Data */
void TaskQueue::GetPacketData() {

    std::string msg = "Worker";

    while (true) {

        Lock(msg);
        Wait(msg);

        /* if the SIGINT event occurs, all worker threads exit the loop */
        if (SIGINT_FLAG) {
            Unlock(msg);
            break;
        }

        //PopTask(); // Critical Section
        PopTaskMessage(); // Critical Section

        Unlock(msg);
    }
}

/* Listening Task Queue */
void TaskQueue::ListeningTaskQueue() {

    std::string msg = "Boss";

    signal(SIGINT, SignalHandler);

    while (true) {

        Lock(msg);

        /* if the SIGINT event occurs, the Boss wake up all threads */
        if (SIGINT_FLAG) {
            std::cout << msg << ": Listening Queue is stopped..." << std::endl;
            pthread_cond_broadcast(&cond);

            Unlock(msg);
            break;
        }

        if (task_queue.empty()) {
            std::cout << "Task Queue is Empty..." << std::endl;
            usleep(1000);

            Unlock(msg);
            continue;
        }

        Signal(msg); // Task Queue is Not Empty

        Unlock(msg);
        usleep(1000);
    }
}

/* Lock */
void TaskQueue::Lock(std::string msg) {

    std::map<int, int> collections;
    collections[1]=2;
    for(auto const &[key,value]: collections)
    {
        if(key==2){};

    }
    //collections.con;

    int is_lock = pthread_mutex_lock(&mutex);

    if (is_lock != 0) {
        std::cout << msg << ": Lock Failed" << std::endl;
        return;
    }
    std::cout << msg << ": Lock Success" << std::endl;
}

/* Unlock */
void TaskQueue::Unlock(std::string msg) {

    int is_unlock = pthread_mutex_unlock(&mutex);

    if (is_unlock != 0) {
        std::cout << msg << ": Unlock Failed\n" << std::endl;
        return;
    }
    std::cout << msg << ": Unlock Success\n" << std::endl;
}

/* Wait */
void TaskQueue::Wait(std::string msg) {

    std::cout << msg << ": Wait For Signal...\n" << std::endl;
    int is_wait = pthread_cond_wait(&cond, &mutex);

    if (is_wait != 0) {
        std::cout << msg << ": Wait Failed" << std::endl;
        return;
    }
    std::cout << msg << ": Receive Signal & Lock Success" << std::endl;
}

/* Signal */
void TaskQueue::Signal(std::string msg) {

    int is_signal = pthread_cond_signal(&cond);

    if (is_signal != 0) {
        std::cout << msg << ": Signal Failed" << std::endl;
        return;
    }
    std::cout << msg << ": Send Signal" << std::endl;
}

