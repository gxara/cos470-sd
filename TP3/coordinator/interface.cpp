#include <deque>
#include <mutex>
#include <iostream>
#include "../helpers/parser.h"
using namespace std;

enum command
{
    command_print_queue,
    command_print_history,
    command_quit,
    command_invalid,
};

command resolveCommand(string inputCommand)
{
    if ((inputCommand == "print_queue") || (inputCommand == "1"))
        return command_print_queue;
    else if ((inputCommand == "quit") || (inputCommand == "2"))
        return command_quit;
    else
        return command_invalid;
}

void handleUserInterface(bool &running, deque<message> &queue, mutex &queueMutex)
{

    string strCommand;
    while (running)
    {
        cout << "> ";
        getline(cin, strCommand);
        switch (resolveCommand(strCommand))
        {
        case command_print_queue:
            cout << "Fila de pedidos:" << endl;
            queueMutex.lock();
            if (queue.size() == 0)
                cout << "vazia." << endl;
            else
                for (int i = 0; i < queue.size(); i++)
                    cout << "  - " << i + 1 << "º: " << queue[i].id << endl;
            queueMutex.unlock();
            break;

        case command_quit:
            cout << "Encerrando programa..." << endl;
            running = false;
            break;

        default:
            cout << "Comando \"" << strCommand << "\" inválido!" << endl;
            break;
        }
    }
}