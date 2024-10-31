#include <cstdio>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


std::string uniqueArr;
std::string sortString(std::string str){
    // declare a string to store the unique characters
    std::string unique = "";
    std::string sortedString = "";
    for(int i=0; i< str.length(); i++){
        if(isalpha(str[i])){
            
            int match =0;
            for(int j=0; j < unique.length(); j++){
                if(unique.at(j)==str.at(i)){
                    match =1;
                    break;
                }
            }
            if(match == 0){
              unique += str[i];
            }
        }
    }
    // copy the unique characters to the global array
    uniqueArr = unique;
    int freq[unique.length()];
    for(int i=0; i< unique.length(); i++){
        freq[i] = 0;
        for(int j=0; j< str.length(); j++){
            if(unique.at(i)==str.at(j)){
                freq[i]++;
            }
        }
        // printf("%d",freq[i]);
    }
    // printf("\n");
    // sort the frequencies in descending order
    for(int i=0; i< unique.length(); i++){
        for(int j=i+1; j< unique.length(); j++){
            if(freq[i] < freq[j]){
                int temp = freq[i];
                freq[i] = freq[j];
                freq[j] = temp;
                char temp2 = unique.at(i);
                unique.at(i) = unique.at(j);
                unique.at(j) = temp2;
            }
        }
        // printf("%d",freq[i]);
    }
    // printf("\n");
    // sort the string based on the frequency of each character
    for(int i=0; i < unique.length(); i++){
        for(int j=0; j < freq[i]; j++){
            sortedString += unique.at(i);
        }
    }
    // printf("\n%s\n",sortedString.c_str());
    return sortedString;
}
std::string splitArr;

void* readServer(void* arg)
{
    int clientSock = *(int*)arg;
    char buffer[1024];
    int n = 0;
    while((n = read(clientSock, buffer, 256)) > 0)
    {
     splitArr = buffer;
    memset(buffer, 0, 1024);
    }
    return NULL;
}

void * readStdin(void * arg){
    int clientSock = *(int*)arg;
    char buffer[256];
    int n = 0;
    std::cout<<"Responder to Server Camera rotary:\n";
    while(fgets(buffer, 256, stdin) != NULL)
    {
        std::string str = buffer;
        std::string sortedString = str; //sortString(str);
        write(clientSock, sortedString.c_str(), sortedString.length());
    }
    return NULL;
}

int main(int argc, char** argv)
{
    int port = 8899;
//    if (argc == 2)
//    {
//        port = atoi(argv[1]);
//    }
//    else
//    {
//        std::cout << "Usage: " << argv[0] << " port" << std::endl;
//        return 1;
//    }
    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    }
    struct sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(port);
    // connect to the server
    if (connect(sock, (struct sockaddr*)&client, sizeof(client)) < 0)
    {
        std::cout << "Error connecting to server" << std::endl;
        return 1;
    }
    // create two pthreads to handle stdin and server input
    pthread_t read_stdin, read_server;
    pthread_create(&read_stdin,0,readStdin, (void *)&sock);
    pthread_create(&read_server, 0, readServer, (void *)&sock);
    // the main thread will be printing the sorted string
    while(1){
        if(splitArr.length() > 0){
            //fprintf(stdout, "%s\n", "SHANNON-FANO-ELIAS Codes:");
            fprintf(stdout," ---- Answer from Server\n %s\n", splitArr.c_str());
            fflush(stdout);
            splitArr = "";
        }
    }
    pthread_join(read_stdin, NULL);
    pthread_join(read_server, NULL);
    return 0;
}
