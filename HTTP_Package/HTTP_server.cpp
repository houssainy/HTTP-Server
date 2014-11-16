#include "HTTP_server.h"

HTTP_server::HTTP_server(int port_number) {
  this->port_number = port_number;
  http_generator = new HTTP_Generator("");
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

  bool httpPlus = false;
  bool timeOut = false;
  // Receive command
  // Get GET request from client
  while(httpPlus && !timeOut) {
    unordered_map<string, char*> values;
    receive_request(clientfd, &values);
    if (values[HTTP_Utils::METHOD_TYPE] == HTTP_Utils::POST) {
      receive_data(clientfd, values[HTTP_Utils::FILE_NAME], atoi(values[HTTP_Utils::CONTENT_LENGTH]));
      send_post_response(clientfd, values[HTTP_Utils::HTTP_TYPE]);
    } else if (values[HTTP_Utils::METHOD_TYPE] == HTTP_Utils::GET){
      send_get_response(clientfd, values[HTTP_Utils::HTTP_TYPE], values[HTTP_Utils::FILE_NAME]);
    }

    httpPlus = values[HTTP_Utils::HTTP_TYPE] == HTTP_Utils::HTTP1 ? true : false;
  }

  close_connection(clientfd);
}

void HTTP_server::receive_request(int clientfd, unordered_map<string, char*> *values) {
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

void HTTP_server::send_get_response(int clientfd, char* http_type, char* requested_path) {
  string response;
  ifstream file(requested_path);
  if (file.is_open()) {
    Dynamic_array data;
    char c;
    while(file.get(c))
      data.insert(c);

    response = http_generator->generate_get_response(http_type, HTTP_Utils::OK, " ", data.size());
    send(clientfd, response.c_str(), response.size());

    send(clientfd, data.get_array(), data.size());
    file.close();
  } else {
    response = http_generator->generate_get_response(http_type, HTTP_Utils::NOT_FOUND, " ", 0);
    send(clientfd, response.c_str(), response.size());
  }
}

void HTTP_server::send_post_response(int clientfd, char* http_type) {
  string response = http_generator->generate_post_response(http_type, HTTP_Utils::OK, " ");
  send(clientfd, response.c_str(), response.size());
}

void HTTP_server::receive_data(int clientfd, char* file_name, int data_length) {
  char buffer[256];
  Dynamic_array data;
  int num_read = 0, offset = 0;
  ofstream out_stream;
  out_stream.open(file_name);
  while ((num_read = read(clientfd, buffer, sizeof(buffer))) > 0) {
    for (int i = 0; i < num_read; i++) {
      out_stream << buffer[i];
      offset++;
      if (offset == data_length) {
          out_stream.close();
          return;
      }
    }
  }
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
