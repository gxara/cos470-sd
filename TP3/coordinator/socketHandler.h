#include <deque>
#include <mutex>

using namespace std;
extern int SOCKET_PORT;
extern int MAX_QUEUED_CONNECTIONS;
extern int MAX_CLIENTS;
extern struct sockaddr_in address;
extern int addressSize; // = sizeof(sockaddr);

struct socketsHandler
{
    int masterSocketFd;
    char *buffer;
    timeval timeout;
    int *clients;
};

void readClientMessage(deque<message> messageQueue, mutex messageQueueMutex, char *buffer, int clientFd);

int createSocket();

void configureSocketToHandleMultipleConnections(int socketFd);

void bindAddressToSocketFd(int socketFd);

void configureSocketIncomingConnections(int socketFd, int maxQueuedConnections);

socketsHandler prepareSocketsHandler();

void socketConnetionsHandlerFactory(bool running,
                                    deque<message> messageQueue,
                                    mutex messageQueueMutex);