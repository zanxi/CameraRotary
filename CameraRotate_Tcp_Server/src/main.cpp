//#include "taskqueue.h"
//#include "threadpool.h"
#include "Server.h"

/* Check Arguments */
int CheckArgs(int args, char** argv) {

    if (args == 3) {

        int queue_size = atoi(argv[1]);
        int pool_size = atoi(argv[2]);

        // if ((queue_size > 0 && queue_size <= MAX_QUEUE_SIZE) && (pool_size > 0 && pool_size <= MAX_POOL_SIZE)) {
        //     return 0;
        // }
    }
    std::cout << "\nusage: ./pool [Queue Size] [Thread Count] // MAX_QUEUE_SIZE == 20, MAX_POOL_SIZE == 10\n" << std::endl;
    return -1;
}

int main(int args, char** argv) {

    // создание подключения к камере
    Camera_Action *ca = new Camera_Action;

    // запуск tcp сервера по порту 8899
    Server *server = Server::Run(8899);
    if (!server) {
        perror("server");
        return 1;
    }
    // запуск слушателя подключаемых клиентов
    while (true) {
        pthread_t thr;
        // ждем клиента ....
        SessionClient *session = server->AcceptSession();
        session->SetCamera(ca); // передает клиентам объект камеру

        //if(session->GetFd())map_sess[session->GetFd()]=session;

        // сеанс связи с клиентом в отдельном потоке
        pthread_create(&thr, NULL, handle_session, session); // handle_session - функция потока связи с клиентом и обработки очереди сообщений, 
        // лежит в server.h
    }

    return 0;
}
