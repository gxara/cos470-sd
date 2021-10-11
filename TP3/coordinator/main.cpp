#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>

#include "../helpers/parser.h"
#include "mutex.h"
#include "socketHandler.h"
#include "interface.h"

using namespace std;

int main(int argc, char **argv)
{
    bool running = true;
    deque<message> messageQueue;
    mutex messageQueueMutex;
    deque<message> queue;
    mutex queueMutex;
    vector<thread> threads;

    thread userInterfaceThread{handleUserInterface, ref(running), ref(queue), ref(queueMutex)};

    threads.push_back(userInterfaceThread);
    threads.push_back(thread(mutexFactory, running, queue, messageQueue, queueMutex, messageQueueMutex));
    threads.push_back(thread(socketConnetionsHandlerFactory, running, messageQueue, messageQueueMutex));

    for (int idx = 0; idx < threads.size(); idx++)
        threads[idx].join();

    return 1;
}