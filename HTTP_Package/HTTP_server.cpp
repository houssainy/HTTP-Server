#include "HTTP_server.h"

HTTP_server::HTTP_server(int portNumber)
{
    //ctor
    tcp_server = new Tcp_server(portNumber);
    tcp_server->set_clients_listner(this);
}

void HTTP_server::start() {
    tcp_server->start();
}

void HTTP_server::onNewClient(int socketfd) {
    cout<< "Client "<< socketfd <<" connected." << endl;
    Dynamic_array data;
    tcp_server->receive(socketfd, &data);
    for(int i = 0; i < data.size(); i++) {
        cout<< data.get_at(i);
    }
    cout << endl;
    /*string msg = tcp_server->receive(socketfd);
    unordered_map<string, string> values = http_parser.parse_msg(msg);
    for (auto& x: values) {
        cout << x.first << ": " << x.second << endl;
    }*/


    tcp_server->close_connection(socketfd);
}

HTTP_server::~HTTP_server()
{
    //dtor
    tcp_server->close_server();
    delete tcp_server;
}
