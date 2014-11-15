#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "../communication_package/Tcp_server.h"

#include "HTTP_Parser.h"
#include "HTTP_Generator.h"
#include "HTTP_Utils.h"

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
        HTTP_Parser http_parser;
};

#endif // HTTP_SERVER_H
