#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <map>

#include "parser.h"
#include "socketHandler.h"

using namespace std;

int SOCKET_PORT = 5000;
int MAX_CLIENTS = 150;
int MAX_QUEUED_CONNECTIONS = 10;
struct sockaddr_in address;
int addressSize = sizeof(sockaddr);

int createSocket()
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM for TCP
    if (socketFd == -1)
    {
        throw "Falha ao abrir o socket.";
    }
    return socketFd;
}

void configureSocketToHandleMultipleConnections(int socketFd)
{
    int opt = 1;
    // Configura socket para aceitar múltiplas conexões
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        throw "Falha ao configurar socket";
    }
}

void bindAddressToSocketFd(int socketFd)
{
    // Abrindo porta para conexões
    address.sin_family = AF_INET;          // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Todas as interfaces de rede
    address.sin_port = htons(SOCKET_PORT); // Porta

    int err = bind(socketFd, (struct sockaddr *)&address, (socklen_t)addressSize);
    if (err != 0)
    {
        cerr << "Error on port " << SOCKET_PORT;
        throw "Erro ao realizar bind na porta";
    }
};

void configureSocketIncomingConnections(int socketFd, int maxQueuedConnections)
{
    // Especifica máximo de conexões na fila
    if (listen(socketFd, maxQueuedConnections) < 0)
    {
        throw "Erro ao especificar máximo de conexões na fila";
    }
}

socketsHandler prepareSocketsHandler()
{

    char buffer[MESSAGE_MAX_SIZE + 1];
    for (int j = 0; j < MESSAGE_MAX_SIZE; j++)
        buffer[j] = '0';

    int masterSocketFd = createSocket();

    configureSocketToHandleMultipleConnections(masterSocketFd);
    bindAddressToSocketFd(masterSocketFd);
    configureSocketIncomingConnections(masterSocketFd, MAX_QUEUED_CONNECTIONS);

    struct timeval timeout;
    timeout.tv_sec = 10;

    socketsHandler x;
    x.masterSocketFd = masterSocketFd;
    x.buffer = buffer;
    x.timeout = timeout;
    x.clients = new int[MAX_CLIENTS]();

    return x;
}
