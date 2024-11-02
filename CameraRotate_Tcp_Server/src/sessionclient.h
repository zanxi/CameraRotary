#ifndef SESSIONCLIENT_H
#define SESSIONCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <stack>

#include "socketobj.h"
#include "camera_action.h"

#include "thrsafe/shd.h"
//#include "mapclientsessions.h"

// класс подключаемой сессии клиента
// создание потока работы с очередью сообщений от клиента


class SessionClient : public SocketObj {
    Camera_Action *ca = nullptr;
    int buf_used;
    static int VALUE;
    pthread_mutex_t val_mut;
    pthread_mutex_t val_queue;

    //std::queue<std::pair<int, std::string>> messageQueue;
    //pthread_t messageQueueThread;
    void* handleMessageQueue();

    std::stack<std::string> task_queue_message_client;
    pthread_t* worker;
    bool start_or_stop = true;
    static void* StartThread(void* object); // поток обработки очереди сообщений
    void StopListening(); // остановить поток обработки очередей
    void GetMessageQueue(); // основная функция запуска бесконечного цикла работы с очередями
    void PopTaskMessage(); // удаляем из очереди сообщение
    void PushTaskMessage(std::string msg); // добавляем в очередь сообщение
    void Lock(std::string msg); // открываем блокировку для работы с камерой
    void Unlock(std::string msg); // закрываем блокировку 


public:
    //char buffer[buf_len];
    SessionClient(sockaddr_in& addr, int fd);
    ~SessionClient();
    //bool EnterInteractiveMode(tsmap<int, SessionClient*> map_sess);
    bool EnterInteractiveMode();
    //bool ProcessMsg(tsmap<int, SessionClient*> map_sess);
    bool ProcessMsg(); // функция содержить всю логику работы с камеров !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void CheckLine(); // добавляем строку между ответами
    void Send(char *msg) const; // посылаем клиенту ответ
    void SetCamera(Camera_Action *_ca); // передаем клиенту камеру под управление

};

//static tsmap<int, SessionClient*> map_sess;
static std::map<int, SessionClient*> map_sess; // коллекция сессий клиентов


#endif // SESSIONCLIENT_H
