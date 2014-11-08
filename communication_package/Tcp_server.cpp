#include "Tcp_server.h"

Tcp_server::Tcp_server(int portNum, Clients_listner *listner) {
    this->portNum = portNum;
    this->listner = listner;
}

void Tcp_server::start() {
    running = true;

    // Open TCP server socket using internet domain
    hello_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (hello_socketfd < 0) {
        printf("Error in opening socket!");
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
        printf("Error on binding!");
        exit(EXIT_FAILURE);
    }

    // Start listening with max 5 backlog queue.
    listen(hello_socketfd,5);

    while(running) {
        clilen = sizeof(cli_addr);
        newsocketfd = accept(hello_socketfd,
                (struct sockaddr *) &cli_addr, &clilen);
        if (newsocketfd < 0) {
            printf("Error in accept!");
            exit(EXIT_FAILURE);
        }
        // TODO(houssiany) open thread and call lisnter.onNewClient

    }
}

void Tcp_server::send(int clientfb, const void* buf) {
    int n = write(clientfb, buf, sizeof(buf));
    if (n < 0)
        printf("Error in sending!");
}

void* Tcp_server::receive(int clientfb){
    bzero(buffer, 256);
    n = read(clientfb, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
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
