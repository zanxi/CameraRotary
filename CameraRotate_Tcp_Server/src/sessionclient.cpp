#include "sessionclient.h"
#include "iostream"
#include <QDebug>

#include "operations.h"

int SessionClient::VALUE = 0;

SessionClient::SessionClient(sockaddr_in& addr, int fd)
    : SocketObj(addr, fd)
    , buf_used(0)
    {
       val_mut = PTHREAD_MUTEX_INITIALIZER;

    }

//bool SessionClient::EnterInteractiveMode(tsmap<int, SessionClient*> map_sess)
bool SessionClient::EnterInteractiveMode()
{
    int rc = read(GetFd(), buffer, buf_len);
    if (rc <= 0)
        return false;

    buf_used = rc;
    CheckLine();

    pthread_mutex_lock(&val_mut); // Because VALUE is
    bool ret = ProcessMsg();      // changeable.
    pthread_mutex_unlock(&val_mut);

    return ret;
}

void SessionClient::Send(char *msg) const
{
    write(GetFd(), msg, strlen(msg));
}

void* SessionClient::handleMessageQueue()
{
    //return nullptr;
    while(true)
    {
        //std::cout << "new MessageQueue "<<"\n";
        while(messageQueue.empty())
            usleep(10000); // Sleep for 10ms

        std::cout << "new MessageQueue "<<"\n";

        // Retrieve and process messages from the message queue
        //pthread_mutex_lock(&mutex); // Lock mutex before accessing shared resources
        std::pair<int, std::string> clientMessagePair = messageQueue.front();
        std::cout << "Message from Client " << clientMessagePair.first << " : " << clientMessagePair.second;
        messageQueue.pop();
        //pthread_mutex_unlock(&mutex); // Unlock mutex after accessing shared resources

    }
}


//bool SessionClient::ProcessMsg(tsmap<int, SessionClient*> map_sess)
bool SessionClient::ProcessMsg()
{
    std::string __buffer = buffer;
    //int pos=-1;
    //std::cout<<"Server receive: "<<buffer;
    qDebug()<<"Server receive (ProcessMsg()): "<<buffer;

    if (strcmp(__buffer.c_str(), "status") == 0) {
        std::string msg = "OK";

        if(!ca.GetState()){
            msg +=std::string(" ") + std::string("idle");
        }
        else
        {
            msg +=std::string(" ") + std::string("active");
        }
        msg+=std::string(" ") + std::to_string(ca.Get_Current_X());
        msg+=std::string(" ") + std::to_string(ca.Get_Current_Y());
        msg+=std::string(" ") + std::to_string(ca.Get_XMAX());
        msg+=std::string(" ") + std::to_string(ca.Get_YMAX());

        std::vector<char> v(msg.begin(),msg.end());
        char* _msg = &v[0];

        Send(_msg);
        //Send(ok_msg);

    } else
    if (__buffer.find("move") != -1)
        {
        int new_x,new_y;
        int curr_x,curr_y;
        std::vector<std::string> vec = Operations::split(__buffer,' ');
        new_x = atoi(vec[1].c_str());
        new_y = atoi(vec[2].c_str());

        curr_x = ca.Get_Current_X();
        curr_y = ca.Get_Current_Y();

        for(auto const &[keyFD,valueSessions]: map_sess)
        {
            if(keyFD!=GetFd())
            {
                //valueSessions->;
                //valueSessions->;
                //valueSessions->;

            }
        }

        std::string msg;
        //Camera_Action ca;
        if(ca.GetState()==true)
        {
            msg = "BUSY\n";
            return true;
        }

        ca.SetMove(true);
        ca.SetState(true);
        qDebug()<<"new x: "<<new_x<<"; new y: "<<new_y<<"\n";
        qDebug()<<"current x: "<<ca.Get_Current_X()<<"; current y: "<<ca.Get_Current_Y()<<"\n";

        if(new_x==ca.Get_Current_X()&&new_y==ca.Get_Current_Y())
        {
            msg = "BUSY\n";
        }
        else
        if(new_x>ca.Get_XMAX()||new_y>ca.Get_YMAX())
            {
                msg = "FAILED\n";
            }
        else
        {            
            msg = "OK\n";
            if(new_x>ca.Get_Current_X())
            {
                for(int i=ca.Get_Current_X();i<new_x;i++)ca.StepLeft();
            }
            if(new_x<ca.Get_Current_X())
            {
                for(int i=ca.Get_Current_X();i>new_x;i--)ca.StepRight();
            }
            if(new_y>ca.Get_Current_Y())
            {
                for(int i=ca.Get_Current_Y();i<new_y;i++)ca.StepUp();
            }
            if(new_y<ca.Get_Current_Y())
            {
                for(int i=ca.Get_Current_Y();i>new_y;i--)ca.StepDown();
            }

            //msg +=std::string(" - send server: ") + std::to_string(new_x);
            //msg +=std::string(" ") + std::to_string(new_y);
        }
        ca.SetState(false);
        ca.SetMove(false);



//        Camera_Action ca;
//        ca.SetState(true); // false - "idle" true - "active"
//        if(ca.GetState()){
//            msg +=std::string(" ") + std::string("idle");
//        }
//        else
//        {
//            msg +=std::string(" ") + std::string("active");
//        }
//        msg+=std::string(" ") + std::to_string(ca.Get_Current_X());
//        msg+=std::string(" ") + std::to_string(ca.Get_Current_Y());
//        msg+=std::string(" ") + std::to_string(ca.Get_XMAX());
//        msg+=std::string(" ") + std::to_string(ca.Get_YMAX());

        std::vector<char> v(msg.begin(),msg.end());
        char* _msg = &v[0];

        Send(_msg);
        //Send(ok_msg);

    }
    else if (strcmp(buffer, "down") == 0) {
        VALUE--;
        //std::string msg_send="receive: "+std::to_string(buffer);
        //Send(ok_msg);
        //Send(msg_send);
        Send(ok_msg);
    } else if (strcmp(buffer, "show") == 0) {
        char *msg = new char[5];
        sprintf(msg, "%d\n", VALUE);
        //std::string msg_send="receive: "+msg;
        //Send(ok_msg);
        //Send(msg_send.c_str());
        Send(msg);
    } else if (strcmp(buffer, "\\q") == 0) { // Close Session
        return false;
    } else {
        Send(unknown_cmd);
    }
    return true;
}

bool SessionClient::ProcessMsg2()
{
    //std::cout<<"Server receive: "<<buffer;
    qDebug()<<"Server receive: "<<buffer;

    if (strcmp(buffer, "up") == 0) {
        VALUE++;
        //std::string msg_send="receive: "+std::to_string(buffer);
        Send(ok_msg);
        //Send(msg_send);
    } else if (strcmp(buffer, "down") == 0) {
        VALUE--;
        //std::string msg_send="receive: "+std::to_string(buffer);
        //Send(ok_msg);
        //Send(msg_send);
        Send(ok_msg);
    } else if (strcmp(buffer, "show") == 0) {
        char *msg = new char[5];
        sprintf(msg, "%d\n", VALUE);
        //std::string msg_send="receive: "+msg;
        //Send(ok_msg);
        //Send(msg_send.c_str());
        Send(msg);
    } else if (strcmp(buffer, "\\q") == 0) { // Close Session
        return false;
    } else {
        Send(unknown_cmd);
    }
    return true;
}



void SessionClient::CheckLine()
{
    for (int i = 0; i < buf_used; i++) {
        if (buffer[i] == '\n') {
            if (i - 1 > 0 && buffer[i-1] == '\r') {
                buffer[i] = 0;
                i--;
            }
            buffer[i] = 0;
            buf_used = i;
            return;
        }
    }
}


