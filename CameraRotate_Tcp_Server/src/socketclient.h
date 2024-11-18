#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <memory>
#include "camera_action.h"

class SocketClient
{
public:
    explicit SocketClient(int mClient,sockaddr_in mAddr);
    ~SocketClient();
    void setCamera(std::shared_ptr<Camera_Action> _dc){ ca= _dc; }

    void run();

private:
    std::shared_ptr<Camera_Action> ca;
    int mClient;
    sockaddr_in mAddr;
    char resp[1024] = {0};

    bool stoprun = false;

    void stopRun(){ stoprun = true; }

    bool ProcessMsg(const char *buf);

};

#endif // SOCKETCLIENT_H
