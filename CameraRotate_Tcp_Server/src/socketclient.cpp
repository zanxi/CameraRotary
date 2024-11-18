#include "socketclient.h"
#include <iostream>
#include "operations.h"

SocketClient::SocketClient(int mClient,sockaddr_in mAddr)
{
    this->mClient = mClient;
    this->mAddr = mAddr;
}

SocketClient::~SocketClient()
{
    int ret = close(mClient);
    if( ret == 0 ){
        std::cout<<"";
    }else{
        std::cout<<"";
    }
}

bool SocketClient::ProcessMsg(const char* buf)
{

    std::string __buffer = buf;

    //if (strcmp(__buffer.c_str(), "status\n") == 0) {
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

        //Send(_msg);
        //Send(ok_msg);

    } else
    if (__buffer.find("move") != -1)
        {

        std::string msg;
        //Camera_Action ca;
        if(ca->GetState()==true)
        {
            msg = "BUSY\n";
            //qDebug()<<" ---------- ERROR MOVE ----------- "<<QString::fromStdString(msg);
            //char* _msg = Operations::string_to_char_s(msg);

            //Send(_msg);
            return true;
        }
        //logger::WriteMsgConsole("Start move");

        int new_x,new_y;
        int curr_x,curr_y;
        std::vector<std::string> vec;// = Operations::split(__buffer,' ');
        new_x = atoi(vec[1].c_str());
        new_y = atoi(vec[2].c_str());

        curr_x = ca->Get_Current_X();
        curr_y = ca->Get_Current_Y();

        ca->SetMove(true);
        ca->SetState(true);
        //qDebug()<<"new x: "<<new_x<<"; new y: "<<new_y<<"\n";
        //qDebug()<<"current x: "<<ca->Get_Current_X()<<"; current y: "<<ca->Get_Current_Y()<<"\n";

        if(new_x==ca->Get_Current_X()&&new_y==ca->Get_Current_Y())
        {
            msg = "BUSY\n";
        }
        else
        if(new_x>ca->Get_XMAX()||new_y>ca->Get_YMAX())
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

        //Send(_msg);
        //Send(ok_msg);

    }
    else if (strcmp(buf, "quit") == 0) { // Close Session

        return false;
    } else {
        //Send(unknown_cmd);

    }
    return true;
}


void SocketClient::run(){
    //char* clientIp = inet_ntoa(mAddr.sin_addr);
    //int clientPort = ntohs(mAddr.sin_port);
    while(!stoprun){
        memset(resp,0,1024);
        char buf[1024] = {0};
        int ret = read(mClient,buf,1024);
        std::cout<<"receive: "<<buf<<"\n";
        if(ret <= 0){
            break;
        }
        std::string bb = std::string(buf)+std::string(" from server ");
        strcpy(resp,bb.data());
        send(mClient,resp,strlen(resp)+1,0);
        std::cout<<"send: "<<bb<<"\n";
    }
}

