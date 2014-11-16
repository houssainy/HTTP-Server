#include "HTTP_server.h"

HTTP_server::HTTP_server(int port_number) {
  this->port_number = port_number;
}

void HTTP_server::start() {
  cout << "Server Starting..." << endl;
  is_running = true;

  // Open TCP server socket using internet domain
  hello_socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (hello_socketfd < 0) {
    cout<< "Error in opening socket!" << endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  // Initialize server configuration.
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_number);

  if (bind(hello_socketfd, (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0) {
    cout<< "Error on binding port " << this->port_number << endl;
    exit(EXIT_FAILURE);
  }

  // Start listening with max 5 backlog queue.
  listen(hello_socketfd, 5);

  cout << "Start Listnening on port " << this->port_number << "..." << endl;
  while(is_running) {
    clilen = sizeof(cli_addr);
    int newsocketfd = accept(hello_socketfd,
            (struct sockaddr *) &cli_addr, &clilen);
    if (newsocketfd < 0) {
      cout << "Error in accept!" << endl;
      exit(EXIT_FAILURE);
    }
    std::thread thread_(&HTTP_server::onNewClient, this, newsocketfd);
    thread_.join(); // TODO(houssainy) remove join to handle multiple connections!
  }
}

void HTTP_server::onNewClient(int clientfd) {
  cout<< "Client "<< clientfd <<" connected." << endl;

  // Receive command
  // Get GET request from client
  unordered_map<string, char*> values;
  receive_get_request(clientfd, &values);

  // TODO(houssainy)
  string msg = "HTTP/1.1 200 OK\r\nContent-Length:10\r\n\r\nMohamedALi\r\n";
  send(clientfd, msg.c_str(), msg.size());
  close_connection(clientfd);
}

void HTTP_server::receive_get_request(int clientfd, unordered_map<string, char*> *values) {
  Dynamic_array char_array;

  char buffer[256];
  int num_read = 0;
  while ((num_read = read(clientfd, buffer, sizeof(buffer))) > 0) {
    for (int i=0 ; i< num_read ; i++) {
      if (i > 0 && buffer[i]=='\r' && char_array.get_at(i-1) == '\n') {
        // Stop when receive empty line.
        // GET /path HTTP/1.0
        // Header .....\r\n
        // \r\n
        http_parser.parse_msg(values, char_array.get_array());
        return;
      }
      char_array.insert(buffer[i]);
      cout << buffer[i];
    }
  }
}

void HTTP_server::receive_post_request(int clientfd, unordered_map<string, char*> *values) {

}

void HTTP_server::send_response(int clientfd, char* requested_path) {

}

void HTTP_server::send(int clientfd, const char* buf, int length) {
  int n = write(clientfd, buf, length);
  if (n < 0)
    cout << "Error while sending data!" << endl;
}

void HTTP_server::close_connection(int clientfb) {
  close(clientfb);
  cout << "Connection " << clientfb << " closed." << endl;
}

// Close the server and all the running connections
void HTTP_server::close_server() {
  is_running = false;
  close(hello_socketfd);

  // TODO(houssainy) close all the running connections to handle HTTP/1.1
  cout << "Server closed." << endl;
}

HTTP_server::~HTTP_server() {
  close_server();
}
