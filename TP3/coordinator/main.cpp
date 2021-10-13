#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <iostream>

#include "parser.h"
#include "socketHandler.h"
using namespace std;

bool running = true;

mutex testMutex;
mutex queueMutex;

deque<message> queue;
deque<message> messageQueue;
vector<thread> coordinatorThreads;

void userInterfaceHandler()
{
    string strCommand;
    cout << "\n******* User IO Handler started ********** " << endl;
    while (running)
    {
        getline(cin, strCommand);
        cout << "Command received: " << strCommand << endl;
        if (strCommand == "1")
        {
            cout << "Requests Queue:" << endl;
            queueMutex.lock();
            if (queue.size() == 0)
                cout << "...empty..." << endl;
            else
                for (int i = 0; i < queue.size(); i++)
                    cout << "  - " << i + 1 << "º: " << queue[i].id << endl;
            queueMutex.unlock();
        }
        else if (strCommand == "2")
        {
            cout << "The program will be shut down" << endl;
            running = false;
        }
        else
        {
            cout << "Invalid command!" << endl;
        }
    }
    cout << "******* Closed  User IO Handler ********** " << endl;
}

void mutualExclusionHandler()
{
    bool locked = false;
    bool gotMessage = false;
    bool queueWork = false;
    message msg;
    cout << "******* Mutual Exclusion Handler started ********** " << endl;
    while (running)
    {
        testMutex.lock();
        if (messageQueue.size() > 0)
        {
            msg = messageQueue.front();
            messageQueue.pop_front();
            gotMessage = true;
        }
        testMutex.unlock();

        if (gotMessage)
        {
            switch (msg.type)
            {
            case MESSAGE_REQUEST:
                queueMutex.lock();
                cout << "--- Got REQUEST message from queue! " << msg.type << endl;
                queue.push_back(msg);
                queueMutex.unlock();
                break;
            case MESSAGE_RELEASE: // Ta faltando mandar o release
                cout << "--- Got RELEASE message from queue! " << msg.type << endl;
                if (!locked)
                {
                    throw "Tried to release unlocked lock";
                }
                locked = false;
                break;
            default:
                throw "Unknown message type";
            }
            gotMessage = false;
        }

        // cout << "--- locked " << msg.type << endl;
        // Process queue
        if (!locked)
        {
            // cout << "--- Will process queue " << msg.type << endl;
            queueMutex.lock();
            if (queue.size() > 0)
            {
                msg = queue.front();
                queue.pop_front();
                queueWork = true;
            }
            queueMutex.unlock();
            if (queueWork)
            {
                cout << "------ Will send message response! " << msg.type << endl;
                char grant[MESSAGE_MAX_SIZE + 1];
                grant[0] = '0' + MESSAGE_GRANT;
                grant[1] = '|';
                for (int i = 2; i <= MESSAGE_MAX_SIZE; i++)
                    grant[i] = '0';
                send(msg.fd, grant, MESSAGE_MAX_SIZE + 1, 0);
                cout << "------ Response sent! " << msg.type << endl;
                locked = true;
                queueWork = false;
            }
        }
    }
    cout << "******* Closed Mutual Exclusion Handler ********** " << endl;
}

void socketConnectionsHandler()
{
    cout << "******* Socket Connections Handler started ********** " << endl;
    socketsHandler coordinatorSocketsHandler = prepareSocketsHandler();
    fd_set readfds;

    while (running)
    {
        FD_ZERO(&readfds);                                          // Clear the sockets list
        FD_SET(coordinatorSocketsHandler.masterSocketFd, &readfds); // Add the master process at the sockets list

        int maxFd = coordinatorSocketsHandler.masterSocketFd;

        // Creating sockets to handle clients connections
        for (int i = 0; i < sizeof(coordinatorSocketsHandler.clients); i++)
        {
            int clientFd = coordinatorSocketsHandler.clients[i];
            if (clientFd > 0)
                FD_SET(clientFd, &readfds);
            if (clientFd > maxFd)
                maxFd = clientFd;
        }

        int activity = select(maxFd + 1, &readfds, NULL, NULL, &coordinatorSocketsHandler.timeout); // Aguarda por atividade em algum socket

        if ((activity < 0) && (errno != EINTR))
        {
            sleep(2);
            continue;
        }

        cout << "- Received activity" << endl;
        // If the master socket has recorded activity it must be a new client connection request
        if (FD_ISSET(coordinatorSocketsHandler.masterSocketFd, &readfds))
        {
            int newConnectionFd = accept(coordinatorSocketsHandler.masterSocketFd, (struct sockaddr *)&address, (socklen_t *)&addressSize);
            if (newConnectionFd < 0)
            {
                throw "Erro ao aceitar nova conexão";
            }
            cout << "New client requested access" << endl;
            for (int i = 0; i < sizeof(coordinatorSocketsHandler.clients); i++)
            {
                if (coordinatorSocketsHandler.clients[i] == 0)
                {
                    coordinatorSocketsHandler.clients[i] = newConnectionFd;
                    cout << "Client added on index " << i << endl;
                    break;
                }
            }
        }

        // Iterate over clients sockets
        for (int i = 0; i < sizeof(coordinatorSocketsHandler.clients); i++)
        {
            int clientFd = coordinatorSocketsHandler.clients[i];

            // Received I/O at one of the sockets
            if (FD_ISSET(clientFd, &readfds))
            {
                char buffer[MESSAGE_MAX_SIZE + 1];

                for (int j = 0; j < MESSAGE_MAX_SIZE; j++)
                    buffer[j] = '0';
                coordinatorSocketsHandler.buffer = buffer;

                cout << "Received I/O at the socket with FD = " << clientFd << endl;
                int valread = read(clientFd, buffer, MESSAGE_MAX_SIZE + 1);

                if (valread == -1)
                {
                    cerr << "Error while reading socket" << endl;
                }
                else if (valread == 0)
                {
                    // Received EOF -> Close connection
                    close(clientFd);
                    clientFd = 0;
                }
                else
                {

                    message tmp = parseMessage(buffer, clientFd);
                    cout << "Message was of the type " << tmp.type << endl;
                    if (tmp.type != -35)
                    {
                        testMutex.lock();
                        messageQueue.push_back(tmp);
                        testMutex.unlock();
                    }
                }
            }
        }
    }
    cout << "******* Closed  Socket Connections Handler ********** " << endl;
}

int main(int argc, char **argv)
{
    coordinatorThreads.push_back(thread(userInterfaceHandler));
    coordinatorThreads.push_back(thread(mutualExclusionHandler));
    coordinatorThreads.push_back(thread(socketConnectionsHandler));
    for (int idx = 0; idx < coordinatorThreads.size(); idx++)
        coordinatorThreads[idx].join();

    return 1;
}