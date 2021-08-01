

#include <sys/types.h>
#include <signal.h>
#include <iostream>

using namespace std;

bool checkIfProcessExists(int pid)
{
    //  If sig is 0 (the null signal), error checking is performed but no signal is actually sent.
    // The null signal can be used to check the validity of pid
    return 0 == kill(pid, 0);
}

int sendSignalToProcess(int pid, int sig)
{
    if (checkIfProcessExists(pid) != 0)
    {
        cout << "Invalid proccess PID " << endl;
        exit(0);
    }

    cout << "Will send a signal " << sig << " for proccess " << pid << endl;

    return kill(pid, sig);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        cout << "The program expects the following command line arguments: pid, signal" << endl;
        exit(0);
    }

    int pid = atoi(argv[1]);
    int signal = atoi(argv[2]);

    sendSignalToProcess(pid, signal);
}