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

#include "socketobj.h"
#include "camera_action.h"

#include "thrsafe/shd.h"
//#include "mapclientsessions.h"


class SessionClient : public SocketObj {
    Camera_Action ca;
    int buf_used;
    static int VALUE;
    pthread_mutex_t val_mut;

    std::queue<std::pair<int, std::string>> messageQueue;
    pthread_t messageQueueThread;
    void* handleMessageQueue();

public:
    char buffer[buf_len];
    SessionClient(sockaddr_in& addr, int fd);
    //bool EnterInteractiveMode(tsmap<int, SessionClient*> map_sess);
    bool EnterInteractiveMode();
    //bool ProcessMsg(tsmap<int, SessionClient*> map_sess);
    bool ProcessMsg();
    bool ProcessMsg2();
    void CheckLine();
    void Send(char *msg) const;



};

//static tsmap<int, SessionClient*> map_sess;
static std::map<int, SessionClient*> map_sess;


#endif // SESSIONCLIENT_H
