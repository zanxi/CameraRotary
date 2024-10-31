#ifndef SOCKETOBJ_H
#define SOCKETOBJ_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

enum {
    listen_queue_len = 16,
    buf_len = 1024,
};


class SocketObj {

    sockaddr_in addr;
    int fd;
public:
    SocketObj(sockaddr_in& addr, int fd)
        : addr(addr), fd(fd)
    {}
    int GetFd() const { return fd; }
    virtual ~SocketObj() { close(fd); }

protected:
    char *ok_msg = "Ok\n";    
    char *unknown_cmd = "FAILED\n";

};


#endif // SOCKETOBJ_H
