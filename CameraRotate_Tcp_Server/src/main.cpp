#include <iostream>
#include "Server.h"

int main() 
{
    try
    {
        Server TCPServer(8899);
        TCPServer.startServer();
    }
    catch(const TCPServerError& ex)
    {
        std::cerr << ex.what() << "\n";
        std::cout << "Server failed to start." << "\n";
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        std::cout << "Server stopped working.\n";
    }
}
