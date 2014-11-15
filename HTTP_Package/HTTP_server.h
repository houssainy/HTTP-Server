#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h> /* exit function */
#include <string.h> /* bzero */
#include <unistd.h>
#include <sys/types.h> /* socket */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* socket */

#include <thread>

#include "HTTP_Parser.h"
#include "HTTP_Generator.h"
#include "HTTP_Utils.h"
#include "../dynamic_array_package/Dynamic_array.h"

using namespace std;

class HTTP_server
{
    public:
        HTTP_server(int portNumber);
        void start();

        virtual ~HTTP_server();
    private:
        int hello_socketfd = -1, portNum = -1;
        bool running = false;
        HTTP_Parser http_parser;

        void send(int clientfd, const char* buf, int length);
        void receive(int clientfd, Dynamic_array *data);
        void close_connection(int clientfb);
        void close_server();

        void onNewClient(int socketfd);
};

#endif // HTTP_SERVER_H
