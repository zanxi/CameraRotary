#include "sessionclient.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <list>

#include "Debug/logger.h"
//#include <QDebug>

#include "operations.h"

int SessionClient::VALUE = 0;

SessionClient::SessionClient(sockaddr_in& addr, int fd)
    : SocketObj(addr, fd)
    , buf_used(0)
    {
       val_mut = PTHREAD_MUTEX_INITIALIZER; // мютекс блокировки читаемых сообщений
       val_queue = PTHREAD_MUTEX_INITIALIZER; // мютекс блокировки работы 

       worker = (pthread_t*)malloc(sizeof(pthread_t));

       // для подключенного клиента создаем поток обработки очереди сообщений
       if (pthread_create(worker, NULL, SessionClient::StartThread, (void*)this) != 0) {           
           exit(0);
       }

    }

SessionClient::~SessionClient()
{
    start_or_stop = false;
    usleep(1000000); // 1 sec
}

//bool SessionClient::EnterInteractiveMode(tsmap<int, SessionClient*> map_sess)
bool SessionClient::EnterInteractiveMode()
{
    char buffer[buf_len];
    int rc = read(GetFd(), buffer, buf_len); // чтение сообщения от клиента
    if (rc <= 0)
        return false;

    buf_used = rc;
    CheckLine();

    pthread_mutex_lock(&val_mut); // Because VALUE is
    //std::cout<<"message: "<<buffer<<"\n";
    PushTaskMessage(buffer); // запись в очередь
      // changeable.
    pthread_mutex_unlock(&val_mut);

    return true;
}

// старт обработки очереди сообщений
void* SessionClient::StartThread(void* object) {
    SessionClient *sess = (SessionClient*)object;
    sess->GetMessageQueue();
}

// остановить поток очереди сообщений
void SessionClient::StopListening()
{
   start_or_stop = false;
}

// бесконечный цикл обработки сообщений
void SessionClient::GetMessageQueue() {

    std::string msg = "Worker";

    while (start_or_stop) {

        while(task_queue_message_client.empty()&&start_or_stop)
        {
            usleep(10000); // Sleep for 10ms
            ////std::cout<<"wait new message ...."<<"\n";
        }
        if(task_queue_message_client.empty())
        {
            usleep(10000); // ждем 10ms появления сообщения
            continue;
            ////std::cout<<"wait new message ...."<<"\n";
        }
        ////std::cout<<"new message .... ----- size ---- "<<task_queue_message_client.size()<<"\n";
        else
        if(!task_queue_message_client.empty()){
           Lock(msg);
           PopTaskMessage(); // критическая секция обработки сообщений и работы с камерой
           Unlock(msg);
        }
    }
}

// посылаем клиенту ответ
void SessionClient::Send(char *msg) const
{
    write(GetFd(), msg, strlen(msg));    
}

void SessionClient::Lock(std::string msg) {

    int is_lock = pthread_mutex_lock(&val_queue);

    if (is_lock != 0) {
        //std::cout << msg << ": Lock Failed" << std::endl;
        return;
    }
    //std::cout << msg << ": Lock Success" << std::endl;
}

/* Unlock */
void SessionClient::Unlock(std::string msg) {

    int is_unlock = pthread_mutex_unlock(&val_queue);

    if (is_unlock != 0) {
        //std::cout << msg << ": Unlock Failed\n" << std::endl;
        return;
    }
    //std::cout << msg << ": Unlock Success\n" << std::endl;
}


void SessionClient::PopTaskMessage()
{
    //buffer = Operations::string_to_char_s(task_queue_message_client.top());
    ProcessMsg();
    task_queue_message_client.pop(); // удаляем последнее сообщение
}


void SessionClient::PushTaskMessage(std::string msg) {

    task_queue_message_client.push(msg);
}


void* SessionClient::handleMessageQueue()
{
    //return nullptr;
    while(true)
    {
        ////std::cout << "new MessageQueue "<<"\n";
//        while(messageQueue.empty())
//            usleep(10000); // Sleep for 10ms

        //std::cout << "new MessageQueue "<<"\n";

        // Retrieve and process messages from the message queue
        //pthread_mutex_lock(&mutex); // Lock mutex before accessing shared resources
//        std::pair<int, std::string> clientMessagePair = messageQueue.front();
//        //std::cout << "Message from Client " << clientMessagePair.first << " : " << clientMessagePair.second;
//        messageQueue.pop();
        //pthread_mutex_unlock(&mutex); // Unlock mutex after accessing shared resources

    }
}


//bool SessionClient::ProcessMsg(tsmap<int, SessionClient*> map_sess)
void SessionClient::SetCamera(Camera_Action *_ca)
{
   ca = _ca;
}

// основная функция - вся логика работы с камерой !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool SessionClient::ProcessMsg()
{   
    
    // берем последнее сообщение
    std::string __buffer = task_queue_message_client.top();
    char buffer[__buffer.length()];
    for(int i=0; i<__buffer.length();i++)buffer[i]=__buffer[i];
    // получить стату камеры
    if (__buffer.find("status") != -1) {

        std::string msg = "OK";

        if(!ca->GetState()){
            msg +=std::string(" ") + std::string("idle");
        }
        else
        {
            msg +=std::string(" ") + std::string("active");
        }
        msg+=std::string(" ") + std::to_string(ca->Get_Current_X());
        msg+=std::string(" ") + std::to_string(ca->Get_Current_Y());
        msg+=std::string(" ") + std::to_string(ca->Get_XMAX());
        msg+=std::string(" ") + std::to_string(ca->Get_YMAX());

        char* _msg = const_cast<char*>(msg.c_str()); // Operations::string_to_char_s(msg);
        logger::WriteMsgConsole(msg);
        //logger::WriteMsgConsole(_msg);
        Send(_msg);
        //Send(ok_msg);

    } else // дать команду камеру на движение
    if (__buffer.find("move") != -1)
        {

        std::string msg;
        //Camera_Action ca;
        if(ca->GetState()==true) // если камера в движении в меомент подачи команды выходим из функции с сообщением ЗАНЯТО
        {
            msg = "BUSY\n";
            //qDebug()<<" ---------- ERROR MOVE ----------- "<<QString::fromStdString(msg);
            char* _msg = Operations::string_to_char_s(msg);
            logger::WriteMsgConsole(msg);
            Send(_msg);
            return true;
        }
        //logger::WriteMsgConsole("Start move");

        int new_x,new_y;
        int curr_x,curr_y;
        std::vector<std::string> vec = Operations::split(__buffer,' ');
        new_x = atoi(vec[1].c_str());
        new_y = atoi(vec[2].c_str());

        curr_x = ca->Get_Current_X();
        curr_y = ca->Get_Current_Y();

        ca->SetMove(true); // ставим статус камеры - active - начало вращения
        ca->SetState(true);// ставим статус камеры - active - начало вращения
        //qDebug()<<"new x: "<<new_x<<"; new y: "<<new_y<<"\n";
        //qDebug()<<"current x: "<<ca->Get_Current_X()<<"; current y: "<<ca->Get_Current_Y()<<"\n";

        if(new_x==ca->Get_Current_X()&&new_y==ca->Get_Current_Y()) // если новые координаты равны текущим то ЗАНЯТО
        {
            msg = "BUSY\n";
        }
        else
        if(new_x>ca->Get_XMAX()||new_y>ca->Get_YMAX()) // если координаты привысили диапазон то ЗАПРЕЩЕНО
            {
                msg = "FAILED\n";                
            }
        else
        {
            if(new_x>ca->Get_Current_X())
            {
                for(int i=ca->Get_Current_X();i<new_x;i++)ca->StepLeft();
            }
            if(new_x<ca->Get_Current_X())
            {
                for(int i=ca->Get_Current_X();i>new_x;i--)ca->StepRight();
            }
            if(new_y>ca->Get_Current_Y())
            {
                for(int i=ca->Get_Current_Y();i<new_y;i++)ca->StepUp();
            }
            if(new_y<ca->Get_Current_Y())
            {
                for(int i=ca->Get_Current_Y();i>new_y;i--)ca->StepDown();
            }
            msg = "OK\n";
            //msg +=std::string(" - send server: ") + std::to_string(new_x);
            //msg +=std::string(" ") + std::to_string(new_y);
        }
        ca->SetState(false);
        ca->SetMove(false);



//        Camera_Action ca;
//        ca->SetState(true); // false - "idle" true - "active"
//        if(ca->GetState()){
//            msg +=std::string(" ") + std::string("idle");
//        }
//        else
//        {
//            msg +=std::string(" ") + std::string("active");
//        }
//        msg+=std::string(" ") + std::to_string(ca->Get_Current_X());
//        msg+=std::string(" ") + std::to_string(ca->Get_Current_Y());
//        msg+=std::string(" ") + std::to_string(ca->Get_XMAX());
//        msg+=std::string(" ") + std::to_string(ca->Get_YMAX());


        char* _msg = Operations::string_to_char_s(msg);
        logger::WriteMsgConsole(msg);
        //logger::WriteMsgConsole("End move");
        //logger::WriteMsgConsole(_msg);
        Send(_msg); // посылаем сообщение OK об удачной операции
        //Send(ok_msg);

    }
    else if (strcmp(buffer, "quit") == 0) { // Close Session
        start_or_stop = false;
        return false;
    } else {
        Send(unknown_cmd);
        logger::WriteMsgConsole(unknown_cmd); // неизвестная команда
    }
    return true;
}

void SessionClient::CheckLine()
{
    if(task_queue_message_client.size()==0)return;
    char *buffer = Operations::string_to_char_s(task_queue_message_client.top());
    for (int i = 0; i < buf_used; i++) {
        if (buffer[i] == '\n') {
            if (i - 1 > 0 && buffer[i-1] == '\r') {
                buffer[i] = 0;
                i--;
            }
            buffer[i] = 0;
            buf_used = i;
            return;
        }
    }
}


