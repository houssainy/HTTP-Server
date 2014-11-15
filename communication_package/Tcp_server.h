#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <stdlib.h> /* exit function */
#include <string.h> /* bzero */
#include <unistd.h>
#include <sys/types.h> /* socket */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* socket */

#include "../dynamic_array_package/Dynamic_array.h"

/**
 * A listner interface to notify the implemented class
 * about the new connected clients.
 **/
class Clients_listner
{
  public:
      virtual void onNewClient(int) = 0;
};

class Tcp_server
{
    public:
        Tcp_server(int portNum); /*Constructor*/

        void set_clients_listner(Clients_listner *clients_listner);
        void start();
        void send(int clientfd, const char* buf, int length);
        void receive(int clientfd, Dynamic_array *data);
        void close_connection(int clientfb);
        void close_server();

        // @Override
        void notifyNewClient(int clientdf);
        virtual ~Tcp_server(); /*Destructor*/
    private:
        int hello_socketfd = -1, portNum = -1;
        bool running = false;
        Clients_listner *clients_listner;
};

#endif // TCP_SERVER_H
