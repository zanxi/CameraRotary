#include <memory>
#include "camera_action.h"

#include <iostream>
#include <thread>
#include <chrono>
#include "RunLoop.h"
#include "server.h"
#include "socketclient.h"

int main(int argc, const char * argv[]) {

    std::shared_ptr<Camera_Action> dc(new Camera_Action);

    task::Runloop* loop = task::Runloop::Create();

    int port = 5555;
    Server srv(port);
    if(!srv.Listen())
    {
        std::cout<<"Bad listen Server: [port:"<<port<<"]\n";
        return 0;
    }
    std::cout<<"Run Server: [port:"<<port<<"]\n";

    while(true)
    {
        srv.Accept();

        task::Clouser funcClient(
            [&srv, &dc]
            {
                SocketClient client(srv.getClientSocket(),srv.getClientAddr());
                client.setCamera(dc);
                client.run();                
            });
        loop->AddRunner(funcClient);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    loop->stopDoLoop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //while(1);
    std::cout<<"\nFINISH APPLICATION\n";
    return 1;
}
