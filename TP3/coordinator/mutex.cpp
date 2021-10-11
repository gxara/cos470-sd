
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <deque>
#include <mutex>
using namespace std;

#include "../helpers/parser.h"

void getGrant(char msg[MESSAGE_MAX_SIZE + 1])
{
    msg[0] = '0' + MESSAGE_GRANT;
    msg[1] = '.';
    for (int i = 2; i <= MESSAGE_MAX_SIZE; i++)
        msg[i] = '0';
}

void mutexFactory(bool &running,
                  deque<message> &queue,
                  deque<message> &messageQueue,
                  mutex &queueMutex,
                  mutex &messageQueueMutex)
{

    bool locked = false;
    bool gotMessage = false;
    bool queueWork = false;
    message msg;
    while (running)
    {
        // Pops a message from queue
        messageQueueMutex.lock();
        if (messageQueue.size() > 0)
        {
            msg = messageQueue.front();
            messageQueue.pop_front();
            gotMessage = true;
        }
        messageQueueMutex.unlock();

        if (gotMessage)
        {
            switch (msg.type)
            {
            case MESSAGE_REQUEST:
                queueMutex.lock();
                queue.push_back(msg);
                queueMutex.unlock();
                break;
            case MESSAGE_RELEASE:
                if (!locked)
                {
                    throw "Tried to released unlocked lock";
                }
                locked = false;
                break;
            default:
                throw "Unknown message type";
            }
            gotMessage = false;
        }

        // Process queue
        if (!locked)
        {
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
                char grant[MESSAGE_MAX_SIZE + 1];
                getGrant(grant);
                send(msg.fd, grant, MESSAGE_MAX_SIZE + 1, 0);
                locked = true;
                queueWork = false;
            }
        }
    }
}