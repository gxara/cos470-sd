#include <deque>
#include <mutex>
using namespace std;

void getGrant(char msg[MESSAGE_MAX_SIZE + 1]);

void mutexFactory(bool running,
                  deque<message> queue,
                  deque<message> messageQueue,
                  mutex queueMutex,
                  mutex messageQueueMutex);
