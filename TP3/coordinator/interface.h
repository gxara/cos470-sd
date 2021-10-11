#include <deque>
#include <mutex>
#include <iostream>
using namespace std;

enum command
{
    command_print_queue,
    command_print_history,
    command_quit,
    command_invalid,
};

command resolveCommand(string inputCommand);

void handleUserInterface(bool running, deque<message> queue, mutex queueMutex);