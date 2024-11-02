#include "server.h"

int main(int argc, char *argv[])
{
    Server *server = Server::Run(8899);
    if (!server) {
        perror("server");
        return 1;
    }
    while (true) {
        pthread_t thr;
        SessionClient *session = server->AcceptSession();

        //if(session->GetFd())map_sess[session->GetFd()]=session;


        pthread_create(&thr, NULL, handle_session, session);
    }

    return 0;

}
