
#include <sys/socket.h>
#include <sys/wait.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "../helpers/timeHelper.h";

void request(int processId) {}

void write(int processId)
{
    ofstream file;
    string timeStamp = getTimestamp();
    file.open("resultado.txt", std::ofstream::out | std::ofstream::app);
    file << timeStamp + "," << processId << std::endl;
    file.close();
}

void release(int processId) {}

int main(int argc, char **argv)
{
    int n = atoi(argv[1]); // número de processos
    int r = atoi(argv[2]); // número de repetições
    int k = atoi(argv[3]); // segundos a aguardar no sleep

    int pids[n];

    for (int i = 0; i < n; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            std::cerr << "Falhou ao forkar processo" << std::endl;
            return -1;
        }
        else if (pids[i] == 0)
        {
            int processId = getpid();
            string timeStamp;
            ofstream file;
            for (int j = 0; j < r; j++)
            {
                request(processId);
                write(processId);
                sleep(k);
                release(processId);
            }
        }
    }
}