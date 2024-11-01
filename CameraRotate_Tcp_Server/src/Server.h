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

#include "sessionclient.h"

/* ------------------------------------------------------- */
/* ------------------------ Server ----------------------- */
/* ------------------------------------------------------- */

class Server : public SocketObj {


    Server(sockaddr_in& addr, int fd)
        : SocketObj(addr, fd)
        {}
public:
    static Server* Run(int port);
    SessionClient *AcceptSession();
};


/* ------------------------------------------------------- */
/* -------------- main and thread function --------------- */
/* ------------------------------------------------------- */

void *handle_session(void *session);


#endif // SERVER_H
