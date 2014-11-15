#include "HTTP_Package/HTTP_server.h"

using namespace std;

int main(int argc, char *argv[]) {
    int portNumber = 8080;
    if(argc > 1) {
        portNumber = atoi(argv[1]);
    }

    HTTP_server httpServer(portNumber);
    httpServer.start();

    return 0;
}
