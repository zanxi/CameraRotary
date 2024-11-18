#include "server.h"
#include <iostream>

Server::Server(int _port)
{
    this->port = _port;    
}

Server::~Server()
{
    ::close(fd);
}

void printIP(const char* name, int addr) {
    printf("%s = %3d.%3d.%3d.%3d\n", name, (addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
           (addr >> 8) & 0xFF, addr & 0xFF);
}


void printIP(const char* name, const sockaddr_in* addr) {
    if (addr != NULL) {
        printIP(name, ntohl(addr->sin_addr.s_addr));
    }
}


bool Server::Listen()
{

    ////////////////////////////////
//struct addrinfo hints, *res;
//int sockfd;
 
//// first, load up address structs with getaddrinfo():
 
//memset(&hints, 0, sizeof hints);
//hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
//hints.ai_socktype = SOCK_STREAM;
//hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
 
//getipv4sourcefilter(NULL, "2000", &hints, &res);
//g
 
// make a socket:
 
//sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
 
// bind it to the port we passed in to getaddrinfo():
 
//bind(sockfd, res->ai_addr, res->ai_addrlen);

//////////////////////////////////////



    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return false;

    sockaddr_in addr;// = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int is_ok = bind(fd, (sockaddr *)&addr, sizeof(addr));
    if (is_ok == -1)
        return false;

    is_ok = listen(fd, 16);
    if (is_ok == -1)
        return false;

    std::cout<< "listen EXCELLENT:" <<"\n";

    std::cout<< " Server Socket Id: " << fd <<"\n";

    std::string s = "Server";
    const char* m = s.c_str();
    printIP(m,&addr);

    //std::cout<< " Server Ip: " << clientIp <<"\n";
    //std::cout<< " Server port: " << clientPort <<"\n";

    return true;

}

void Server::Accept()
{
    //sockaddr_in addr;
    clientAddr = {};
    socklen_t len = sizeof(clientAddr);
    std::cout<< "New Client:" <<"\n";
    memset(&clientAddr,0,sizeof(clientAddr));
    mClientSocket = ::accept(fd, (sockaddr *)&clientAddr, &len);
    //clientIp = ntohl(clientAddr.sin_addr);
    clientPort = ntohs(clientAddr.sin_port);
    //std::cout<< " Server Ip: " << clientIp <<"\n";
    std::cout<< " Server port: " << clientPort <<"\n";
    printIP("Client",&clientAddr);
}
