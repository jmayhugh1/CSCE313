#include "TCPRequestChannel.h"
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;


TCPRequestChannel::TCPRequestChannel (const std::string _ip_address, const std::string _port_no) {
    //if server
    //      create socket on port
    //          -specify domain, tyoe and protocol
    //      bind socket to address  set-ups listening
    //      mark socket as listening
    if (_ip_address == ""){
        struct sockaddr_in server;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0){
            perror("socket");
            exit(1);
        }

        //memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        cout << "port no: " << _port_no << endl;
        server.sin_port = htons(stoi(_port_no));
        cout << "done" << endl;

        int bind_result = bind(sockfd, (struct sockaddr *) &server, sizeof(server));

        listen(sockfd, 2048);

    }
    

    // if client
    //      create socket
    //      - specify domain, type and protocol
    //      connect socket to address
    else{
        struct sockaddr_in server;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0){
            perror("socket");
            exit(1);
        }

        //memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(stoi(_port_no));
        inet_pton(AF_INET, _ip_address.c_str(), &server.sin_addr);
        connect(sockfd, (struct sockaddr *) &server, sizeof(server));
    }
}

TCPRequestChannel::TCPRequestChannel (int _sockfd) {
sockfd = _sockfd;
}

TCPRequestChannel::~TCPRequestChannel () {
    //close socket
    close(sockfd);


}

int TCPRequestChannel::accept_conn () {
    // implementing the accept function for the server
    //      accept connection
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    return accept(sockfd, (struct sockaddr *) &client, &client_len);
    //      return new socket



}
//read/write, recv/send
int TCPRequestChannel::cread (void* msgbuf, int msgsize) {

return read(sockfd, msgbuf, msgsize);
}

int TCPRequestChannel::cwrite (void* msgbuf, int msgsize) {
    return write(sockfd, msgbuf, msgsize);
}
