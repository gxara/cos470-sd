#include <deque>
#include <mutex>

using namespace std;
int SOCKET_PORT = 5000;
int MAX_QUEUED_CONNECTIONS = 10;
struct sockaddr_in address;
int addressSize = sizeof(sockaddr);

struct socketsHandler
{
    int masterSocketFd;
    char *buffer;
    timeval timeout;
    int *clients;
};

int readClientMessage(deque<message> messageQueue, mutex messageQueueMutex, char *buffer, int clientFd);

int createSocket();

void configureSocketToHandleMultipleConnections(int socketFd);

int bindAddressToSocketFd(int socketFd);

void configureSocketIncomingConnections(int socketFd, int maxQueuedConnections);

socketsHandler prepareSocketsHandler();

void socketConnetionsHandlerFactory(bool running,
                                    deque<message> messageQueue,
                                    mutex messageQueueMutex);