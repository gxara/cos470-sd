#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <map>

#include "../helpers/parser.h"

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

int readClientMessage(deque<message> messageQueue, mutex messageQueueMutex, char *buffer, int clientFd)
{
    message tmp = parseMessage(buffer, clientFd);
    if (tmp.type != -35)
    {
        messageQueueMutex.lock();
        messageQueue.push_back(tmp);
        messageQueueMutex.unlock();
    }
}

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

int bindAddressToSocketFd(int socketFd)
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

    int masterSocketFd = createSocket();

    configureSocketToHandleMultipleConnections(masterSocketFd);
    bindAddressToSocketFd(masterSocketFd);
    configureSocketIncomingConnections(masterSocketFd, MAX_QUEUED_CONNECTIONS);

    // Configurando timeout do select
    struct timeval timeout;
    timeout.tv_sec = 1;

    socketsHandler x;
    x.masterSocketFd = masterSocketFd;
    x.buffer = buffer;
    x.timeout = timeout;
    x.clients = {0}; // All positions initialized as zero

    return x;
}

// Thread to handle new socket connections
void socketConnetionsHandlerFactory(bool &running,
                                    deque<message> &messageQueue,
                                    mutex &messageQueueMutex)
{
    socketsHandler socketsHandler = prepareSocketsHandler();
    fd_set readfds;

    while (running)
    {
        FD_ZERO(&readfds);                               // Clear the sockets list
        FD_SET(socketsHandler.masterSocketFd, &readfds); // Add the master process at the sockets list

        int maxFd = socketsHandler.masterSocketFd;

        // Creating sockets to handle clients connections
        for (int i = 0; i < sizeof(socketsHandler.clients); i++)
        {
            int clientFd = socketsHandler.clients[i];
            if (clientFd > 0)
                FD_SET(clientFd, &readfds);
            if (clientFd > maxFd)
                maxFd = clientFd;
        }

        int activity = select(maxFd + 1, &readfds, NULL, NULL, &socketsHandler.timeout); // Aguarda por atividade em algum socket

        if ((activity < 0) && (errno != EINTR))
            cout << "Erro no comando select" << endl;

        // If the master socket has recorded activity it must be a new client connection request
        if (FD_ISSET(socketsHandler.masterSocketFd, &readfds))
        {
            int newConnectionFd = accept(socketsHandler.masterSocketFd, (struct sockaddr *)&address, (socklen_t *)&addressSize);
            if (newConnectionFd < 0)
            {
                throw "Erro ao aceitar nova conexão";
            }
            for (int i = 0; i < sizeof(socketsHandler.clients); i++)
            {
                if (socketsHandler.clients[i] == 0)
                {
                    socketsHandler.clients[i] = newConnectionFd;
                    break;
                }
            }
        }

        // Iterate over clients sockets
        for (int i = 0; i < sizeof(socketsHandler.clients); i++)
        {
            int clientFd = socketsHandler.clients[i];

            // Received I/O at one of the sockets
            if (FD_ISSET(clientFd, &readfds))
            {
                int valread = read(clientFd, socketsHandler.buffer, MESSAGE_MAX_SIZE + 1);
                if (valread == -1)
                {
                    cerr << "Erro na leitura do socket" << endl;
                }
                if (valread == 0)
                {
                    // Received EOF -> Close connection
                    close(clientFd);
                    clientFd = 0;
                }
                else
                {
                    readClientMessage(messageQueue, messageQueueMutex, socketsHandler.buffer, clientFd);
                }
            }
        }
    }
};
