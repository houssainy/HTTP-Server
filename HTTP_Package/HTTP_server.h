#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <fstream>
#include "../communication_package/Tcp_server.h"

using namespace std;
class HTTP_server : public Clients_listner
{
    public:
        HTTP_server(int portNumber);
        void start();
        void onNewClient(int socketfd);
        virtual ~HTTP_server();
    private:
        Tcp_server *tcp_server;
};

#endif // HTTP_SERVER_H
