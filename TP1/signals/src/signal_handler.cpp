#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <typeinfo>
#include <iostream>
#include <csignal>
#include <string.h>

using namespace std;

void sigABRTsignalHandler(int signum)
{
    cout << "\n Signal ABRT (" << signum << ") received.\n";
}

void sigFPEsignalHandler(int signum)
{
    cout << "\n Signal FPE (" << signum << ") received.\n";
}

void sigINTsignalHandler(int signum)
{
    cout << "\n Signal INT (" << signum << ") received. The execution will stop\n";
    exit(signum);
}

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        cout << "The program expects the following command line argument: wait_type" << endl;
        cout << "Wait type must be busy (for busy waiting) or block (for blocking wait)" << endl;
        return 0;
    }

    // register signal SIGABRT and handler
    signal(SIGABRT, sigABRTsignalHandler);

    // register signal SIGFPE and handler
    signal(SIGFPE, sigFPEsignalHandler);

    // register signal SIGINT and handler
    signal(SIGINT, sigINTsignalHandler);

    cout << "This process ID is: " << getpid() << "\n";
    cout << "The wait type is: " << argv[1] << "\n";

    if (strcmp(argv[1], "block") == 0)
    {
        while (true)
        {
            cout << "Waiting for a signal... " << endl;
            sleep(4);
        }
    }
    else if (strcmp(argv[1], "busy") == 0)
    {
        while (true)
        {
            cout << "Waiting for a signal... " << endl;
        }
    }
    else
    {
        cout << "Wait type must be busy (for busy waiting) or block (for blocking wait)" << endl;
        return 0;
    }

    return 0;
}