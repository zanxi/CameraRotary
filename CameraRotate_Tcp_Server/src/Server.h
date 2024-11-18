#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>


class Server
{
private:    
    int fd = 0;//INVALID_SOCKET;
    int port;

    int mClientSocket;
    char* clientIp;
    sockaddr_in clientAddr;
    int clientPort;


public:
    explicit Server(int _port);// I forget to add a seperate start() QAQ
    ~Server();

    bool Listen();
    void Accept();

    char* getClientIp(){ return clientIp; };
    sockaddr_in getClientAddr(){ return clientAddr; };
    int getClientPort(){ return clientPort; };
    int getClientSocket(){ return mClientSocket; };
};

#endif // SERVER_H
