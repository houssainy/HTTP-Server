#include "Tcp_server.h"
#include <thread>
#include <iostream>

using namespace std;

Tcp_server::Tcp_server(int portNum) {
    this->portNum = portNum;
}

void Tcp_server::set_clients_listner(Clients_listner *clients_listner) {
    this->clients_listner = clients_listner;
}

void Tcp_server::start() {
    cout << "Server Starting..." << endl;
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
        cout<< "Error on binding port " << this->portNum << endl;
        exit(EXIT_FAILURE);
    }

    // Start listening with max 5 backlog queue.
    listen(hello_socketfd,5);

    cout << "Start Listnening on port " << this->portNum
         << "..." << endl;
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
    if(this->clients_listner != NULL)
        clients_listner->onNewClient(clientdf);
}

void Tcp_server::send(int clientfd, const char* buf, int length) {
    int n = write(clientfd, buf, length);
    if (n < 0)
        cout << "Error while sending data!" << endl;
}

void Tcp_server::receive(int clientfd, Dynamic_array *data){
    char buffer[256];

    int num_read = 0;
    while ((num_read = read(clientfd, buffer, sizeof(buffer))) > 0) {
        for(int i = 0; i < num_read; i++)
            data->insert(buffer[i]);
    }
}

/**
 * Close connection with the given client
 */
void Tcp_server::close_connection(int clientfb){
    close(clientfb);
    cout << "Connection " << clientfb << " closed." << endl;
}

void Tcp_server::close_server() {
    running = false;
    close(hello_socketfd);
    cout << "Server closed." << endl;
}

Tcp_server::~Tcp_server()
{
    //dtor
}
