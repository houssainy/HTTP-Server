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

    // TODO(houssiany) handle this method
    cout << "Client: " << tcp_server->receive(socketfd) << endl;
    char msg[] = "Hello from Server!";
    tcp_server->send(socketfd, msg, sizeof(msg));

    // Send file
    string line ;
    ifstream myfile("houssainy.jpg");
    if (myfile.is_open()) {
        ofstream outfile;
        outfile.open("example.jpg");

        while(getline(myfile, line)) {
            cout << line << " Size = " << line.size()<< endl;
            const char*data = line.c_str();
            outfile << data << endl;
          //  tcp_server->send(socketfd, line.c_str(), line.size());
        }

        outfile.close();

        string endOffile = "END";
        tcp_server->send(socketfd, endOffile.c_str(), endOffile.size());
        myfile.close();
    } else
        cout << "Unable to open file";

    tcp_server->close_connection(socketfd);
    tcp_server->close_server();
}

HTTP_server::~HTTP_server()
{
    //dtor
    delete tcp_server;
}
