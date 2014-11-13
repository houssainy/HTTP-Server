#include <iostream>
#include "communication_package/Tcp_server.h"


#include <fstream>

using namespace std;

Tcp_server *tcp_server;
void receiveMessage(int socketfd) {
    cout << "Client: " << tcp_server->receive(socketfd) << endl;
    char msg[] = "Hello from Server!";
    tcp_server->send(socketfd, msg, sizeof(msg));

    // Send file
    string line;
    ifstream myfile("test.txt");
    if (myfile.is_open()) {
        /*int fileSize = myfile.tellg();
		myfile.seekg(0, ios::beg);

        // send file size
        string sFileSize = std::to_string(fileSize);
        tcp_server->send(socketfd, sFileSize.c_str(), sizeof(msg));*/

        while(getline(myfile,line)) {
          tcp_server->send(socketfd, line.c_str(), line.size());
        }
        string endOffile = "\0";
        tcp_server->send(socketfd, endOffile.c_str(), endOffile.size());
        myfile.close();
    } else
        cout << "Unable to open file";

    tcp_server->close_connection(socketfd);
    tcp_server->close_server();
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
    /*if(argc > 1) {
        portNumber = atoi(argv[1]);
    }*/

    ListnerHandler * listner = new ListnerHandler();

    tcp_server= new Tcp_server(portNumber);
    tcp_server->set_clients_listner(listner);
    tcp_server->start();

    return 0;
}
