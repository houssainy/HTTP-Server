#include "Tcp_server.h"
#include <thread>
#include <iostream>

using namespace std;

Tcp_server::Tcp_server(int portNum, Clients_listner *listner) {
    this->portNum = portNum;
    this->listner = listner;
}

void Tcp_server::start() {
    running = true;

    // Open TCP server socket using internet domain
    hello_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (hello_socketfd < 0) {
        cout<< "Error in opening socket!" << endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Initialize server configuration.
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNum);

    if (bind(hello_socketfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {
        cout<< "Error on binding!" << endl;
        exit(EXIT_FAILURE);
    }

    // Start listening with max 5 backlog queue.
    listen(hello_socketfd,5);

    cout << "Waiting for clients..." << endl;
    while(running) {
        clilen = sizeof(cli_addr);
        int newsocketfd = accept(hello_socketfd,
                (struct sockaddr *) &cli_addr, &clilen);
        if (newsocketfd < 0) {
            cout << "Error in accept!" << endl;
            exit(EXIT_FAILURE);
        }
        std::thread thread_(&Tcp_server::notifyNewClient, this, newsocketfd);
        thread_.join();
    }
}

void Tcp_server::notifyNewClient(int clientdf) {
    if(this->listner != NULL)
        listner->onNewClient(clientdf);
}

void Tcp_server::send(int clientfb, const void* buf, int length) {
    int n = write(clientfb, buf, length);
    if (n < 0)
        cout<< "Error in sending!" << endl;
}

void* Tcp_server::receive(int clientfb){
    unsigned char buffer[256];
    bzero(buffer, 256);
    int n = read(clientfb, buffer, 255);
    if (n < 0) {
        cout << "ERROR in reading from socket!" << endl;
        return NULL;
    }
    return buffer;
}

/**
 * Close connection with the given client
 */
void Tcp_server::close_connection(int clientfb){
    close(clientfb);
}

void Tcp_server::close_server() {
    running = false;
    close(hello_socketfd);
}

Tcp_server::~Tcp_server()
{
    //dtor
}
