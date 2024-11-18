//
//  main.cpp
//  TaskLoop
//
//  Created by pansafeimager on 15/12/9.
//  Copyright © 2015年 imager. All rights reserved.
//

#include <memory>
#include "camera_action.h"

#include <iostream>
#include <thread>
#include <chrono>
#include "RunLoop.h"
#include "server.h"
#include "socketclient.h"

void Func()
{
    std::cout<<"hello world!" << std::endl;
}

int Add(int a, int b){
    std::cout<< "call Add()" << std::endl;
    return a + b;
}
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


    task::Clouser func([]{Func();});
    task::Clouser clouser([]{Add(1,2);});

    loop->AddRunner(clouser);
    loop->AddRunner(clouser);
    loop->AddRunner(clouser);
    loop->AddRunner(clouser);
    loop->AddRunner(func);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    loop->stopDoLoop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //while(1);
    std::cout<<"\nFINISH APPLICATION\n";
    return 1;
}
