#include <iostream>
#include "communication_package/Tcp_server.h"

using namespace std;

Tcp_server *tcp_server;
void receiveMessage(int socketfd) {
    cout << "Client: " << tcp_server->receive(socketfd) << endl;
    char msg[] = "Hello from Server!";
    tcp_server->send(socketfd, msg, sizeof(msg));
    //tcp_server->close_connection(socketfd);
    //tcp_server->close_server();
}

class ListnerHandler : public Clients_listner {
    public:
        void onNewClient(int socketfd) {
            cout<< "Client "<< socketfd <<" connected." << endl;
            receiveMessage(socketfd);
        }
};

int main(int argc, char *argv[]) {
    int portNumber = 8080;
    if(argc > 1) {
        portNumber = atoi(argv[1]);
    }

    cout << "Server Started on port " << portNumber << endl;
    ListnerHandler * listner = new ListnerHandler();

    tcp_server= new Tcp_server(portNumber);
    tcp_server->set_clients_listner(listner);
    tcp_server->start();

    return 0;
}
