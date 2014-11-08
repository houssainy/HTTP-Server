#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <stdlib.h> /* exit function */
#include <strings.h> /* bzero */
#include <sys/types.h> /* socket */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* socket */


/**
 * A listner interface to notify the implemented class
 * about the new connected clients.
 **/
class Clients_listner
{
  public:
      virtual int onNewClient(int) = 0;
};

class Tcp_server
{
    public:
        Tcp_server(int portNum, Clients_listner *listner); /*Constructor*/
        void start();
        void send(int clientfb, const void* buf);
        void* receive(int clientfb);
        void close_connection(int clientfb);
        void close_server();
        virtual ~Tcp_server(); /*Destructor*/
    private:
        int hello_socketfd = -1, newsocketfd = -1, portNum = -1;
        bool running = false;
        Clients_listner *listner;
};

#endif // TCP_SERVER_H
