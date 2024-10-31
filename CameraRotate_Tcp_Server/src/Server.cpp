#include "Server.h" // Including the header file to define the Server class and related errors
#include <iostream> // Including the library for standard input/output operations
#include <cstring> // This header file is included to use C-style string manipulation functions like memset
#include <unistd.h> // This header file is included for POSIX operating system API, which is used for functions like usleep
#include <sys/socket.h> // This header file is included for socket-related functions and structures used in network programming, 
                        // such as socket, bind, listen, and accept

#include <chrono>
#include <thread>

#include "operations.h"

//using namespace std::chrono_literals;

#define STATIC

// Structure to hold data for POSIX threads
struct PosixThreadData
{
    PosixThreadData(Server* srv, int clientSocket) : server(srv), clientSoc(clientSocket) {}
    Server* server; // Pointer to a Server object
    int clientSoc; // Client socket descriptor
};

// Constructor for the Server class, taking a port number as argument
Server::Server(int Port) : serverPort(Port)
{   
    try
    {
        createAndBindSocket(); // Creating and binding the socket for the server

        // Creating a thread for handling the message queue
        if(pthread_create(&messageQueueThread, NULL, handleMessageQueueWrapper, (void *)this) != 0)
        {
            throw TCPServerError("Message Queue Thread could not be created."); // Throw an error if thread creation fails
        }
    }
    catch(const TCPServerError& ex)
    {
        close(serverSocket); // Clean up resources in case of exception
        throw; // Re-throw the exception
    }
}

// Destructor for the Server class
Server::~Server()
{
    // Join threads for all connected clients
    for(auto& [socket, thread]: clientsMap)
    {
        if(pthread_join(thread, NULL) != 0)
        {
            std::cerr << "Failed to join thread.\n"; // Print error message if joining thread fails
        }

        close(socket);
    }
  
    if(pthread_join(messageQueueThread, NULL) != 0)
    {
        std::cerr << "Failed to join thread.\n"; // Print error message if joining thread fails
    }
  
    close(serverSocket); // Closing the server socket when the Server object is destroyed
}

// Function to create and bind the socket for the server
void Server::createAndBindSocket()
{
    close(serverSocket); // Closing any existing socket before creating and binding a new one

    // Creating a socket for communication using IPv4 and TCP protocol
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw TCPServerError("Socket could not be created."); // Throw an error if socket creation fails
    }

    // Initializing the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // Using IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accepting connections from any IP address
    serverAddr.sin_port = htons(serverPort); // Setting the server port

    // Binding the socket to the server address
    if(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        close(serverSocket);
        throw TCPServerError("Unable to bind socket."); // Throw an error if binding fails
    }
}

// Function to start the server
void Server::startServer()
{
    bool dec = true; // Decision variable
    
    // Main loop to start and restart the server
    while(dec)
    {
        try
        {
            startListening(); // Starting to listen for incoming connections
        }
        catch(const TCPServerError ex)
        {
            std::cerr << ex.what() << '\n'; // Printing error message if an exception is caught
        }

        return;
        
        std::cout << "Do you want to try to start the server again? [Y/N]";
        std::string ans{}; // Answer
        int counter = 0;
        while(ans != "Y" && ans != "y" && ans != "N" && ans != "n")    
        {
            if(counter >= 1)
            {
                std::cout << "Invalid answer, please enter your answer again: ";
            }
            std::cin >> ans; // Getting user input for restarting the server
            ++counter;
        }
        
        if(ans == "Y" || ans == "y")
        {
            dec = true; // Setting decision to true for restarting the server
            clientsMap.clear(); // Clearing the map of connected clients
            createAndBindSocket(); // Recreating and binding the socket 
        }
        else
        {
            dec = false; // Setting decision to false for shutting down the server
            std::cout << "Server is shutting down.\n"; // Printing shutdown message
        }
    }  
}

// Function to handle the message queue in a separate thread
void* Server::handleMessageQueue()
{
    //return nullptr;
    while(true)
    {
        //std::cout << "new MessageQueue "<<"\n";
        while(messageQueue.empty())
        {
            usleep(10000); // Sleep for 10ms
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "new MessageQueue "<<"\n";

        {
        // Retrieve and process messages from the message queue
        //pthread_mutex_lock(&mutex); // Lock mutex before accessing shared resources
           std::pair<int, std::string> clientMessagePair = messageQueue.front();
           std::cout << "Message from Client " << clientMessagePair.first << " : " << clientMessagePair.second;

           ProcessMsg(clientMessagePair.first, clientMessagePair.second.data());

           messageQueue.pop();
        //pthread_mutex_unlock(&mutex); // Unlock mutex after accessing shared resources

        }



    }
}


//bool Server::ProcessMsg(char* buffer)
bool Server::ProcessMsg(int _fd, std::string buffer)
{
    std::string __buffer = buffer;
    //int pos=-1;
    //std::cout<<"Server receive: "<<buffer;
    //()<<"Server receive (ProcessMsg()): "<<QString::fromStdString(buffer);

    if (strcmp(__buffer.c_str(), "status\n") == 0) {
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

        Send(_fd, _msg);
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

        //for(auto const &[keyFD,valueSessions]: map_sess)
        {
            //if(keyFD!=GetFd())
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
        //qDebug()<<"new x: "<<new_x<<"; new y: "<<new_y<<"\n";
        //qDebug()<<"current x: "<<ca.Get_Current_X()<<"; current y: "<<ca.Get_Current_Y()<<"\n";

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

        std::vector<char> v(msg.begin(),msg.end());
        char* _msg = &v[0];

        Send(_fd, _msg);
        //Send(ok_msg);

    }
//    else if (strcmp(buffer, "\\q") == 0) { // Close Session
//        return false;
//    }
    else {
        Send(_fd, unknown_cmd);
    }
    return true;
}


void Server::Send(int fd, char *msg) const
{
    //write(GetFd(), msg, strlen(msg));
    write(fd, msg, strlen(msg));
}



// Static function wrapper for handling the message queue in a separate thread
STATIC void* Server::handleMessageQueueWrapper(void* arg)
{
    Server* instance = reinterpret_cast<Server*>(arg);
    instance->handleMessageQueue(); // Call the non-static member function to handle the message queue
}

// Function to start listening for incoming connections
void Server::startListening()
{
    if (listen(serverSocket, MAX_CLIENTS) == -1) 
    {
        throw TCPServerError("Listening error."); // Throw an error if listening fails
    }

    std::cout << "Server is listening for connections on Port " << serverPort << "\n"; // Print listening message

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Accept incoming client connections and spawn threads to handle them
    while(true)
    {
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if(clientSocket == -1)
        {
            std::cerr << "Failed to accept request from a client." << "\n"; // Print error message if accepting client fails
        }
        else
        {
            std::cout << "Client " << clientSocket << " connected.\n"; // Print client connection message
        }

        pthread_t thread;
        PosixThreadData threadData(this, clientSocket); // Create thread data structure
        if(pthread_create(&thread, NULL, handleClientWrapper, (void *)&threadData) != 0)
        {
            throw TCPServerError("Thread could not be created."); // Throw an error if thread creation fails
            close(clientSocket); // Close client socket if thread creation fails
        }
        else
        {
            clientsMap[clientSocket] = thread; // Add client socket and thread to the map
            //messageQueueMap[clientSocket] =
        }
    }
}

// Function to handle a client connection
void* Server::handleClient(int clientSocket)
{
    std::cout<<"handleClient(int clientSocket): "<<clientSocket<<"\n";


    char buffer[256]; // Buffer to store received data
    int bytesRead = 0; // Number of bytes read

    // Receive data from the client until connection is closed
    while((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
    {
        //pthread_mutex_lock(&mutex); // Lock mutex before accessing shared resources
        //PTHREAD_MUTEX_RECURSIVE;
        std::cout<<"pthread_mutex_lock(&mutex);: "<<"\n";
        messageQueue.push(std::make_pair(clientSocket, buffer)); // Push received data to the message queue
        //std::cout<<"receive message: "<<buffer<<"\n";
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        //pthread_mutex_unlock(&mutex); // Unlock mutex after accessing shared resources
        //memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    }

    if(bytesRead <= 0)
    {
        std::cout << "Client " << clientSocket << " disconnected." << "\n";
    }

    close(clientSocket);
}

// Static function wrapper for handling client connections
STATIC void* Server::handleClientWrapper(void* arg)
{
    std::cout<<"handleClientWrapper(void* arg): "<<"\n";
    PosixThreadData* threadData = reinterpret_cast<PosixThreadData*>(arg);
    threadData->server->handleClient(threadData->clientSoc);
}
