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
#include <unordered_map>

#include "HTTP_Parser.h"
#include "HTTP_Generator.h"
#include "HTTP_Utils.h"
#include "../dynamic_array_package/Dynamic_array.h"

using namespace std;

/**
* This class is an ipmlementation for a simple HTTP Server.
*
* This server handle the basic HTTP requests (GET, POST) and their responses.
*/
class HTTP_server {
  public:
    HTTP_server(int port_number);
    // Start the server and stay running until close is called.
    void start();
    void close_server();
    virtual ~HTTP_server();
  private:
    int hello_socketfd = -1;
    int port_number = -1;
    bool is_running = false;
    HTTP_Parser http_parser;
    HTTP_Generator *http_generator; // TODO(houssainy)

    int thread_count = 0;
    std::thread threads[10];
    // This function will be called for each new client connect to the server to
    // receive it requests and handle it.
    void onNewClient(int socketfd);

    void receive_request(int clientfd, unordered_map<string, char*> *values);

    void send_get_response(int clientfd, char* http_type, char* requested_path);
    void send_post_response(int clientfd, char* http_type);
    void receive_data(int clientfd, char* file_name, int data_length);

    void send(int clientfd, const char* buf, int length);
    // Close connection with the given client
    void close_connection(int clientfb);
};

#endif // HTTP_SERVER_H
