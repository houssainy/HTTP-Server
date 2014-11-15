#include "HTTP_server.h"

HTTP_server::HTTP_server(int portNum)
{
    //ctor
    this->portNum = portNum;
}

void HTTP_server::start() {
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
        std::thread thread_(&HTTP_server::onNewClient, this, newsocketfd);
        thread_.join();
    }
}

void HTTP_server::onNewClient(int socketfd) {
    cout<< "Client "<< socketfd <<" connected." << endl;

    // Receive command
    Dynamic_array data;
    receive(socketfd, &data);
    for(int i = 0; i < data.size(); i++) {
        cout<< data.get_at(i);
    }
    cout << endl;
    cout << "Received" << endl;
    string msg = "HTTP/1.1 200 OK\r\nContent-Lenght:10\r\n\r\nMohamedALi\r\n";
    send(socketfd, msg.c_str(), msg.size());
    close_connection(socketfd);
}

void HTTP_server::send(int clientfd, const char* buf, int length) {
    int n = write(clientfd, buf, length);
    if (n < 0)
        cout << "Error while sending data!" << endl;
}

void HTTP_server::receive(int clientfd, Dynamic_array *data){
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
void HTTP_server::close_connection(int clientfb){
    close(clientfb);
    cout << "Connection " << clientfb << " closed." << endl;
}

void HTTP_server::close_server() {
    running = false;
    close(hello_socketfd);
    cout << "Server closed." << endl;
}

HTTP_server::~HTTP_server()
{
    //dtor
    close_server();
}
