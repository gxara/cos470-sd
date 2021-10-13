
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
#include <mutex>
#include "../helpers/timeHelper.h"
#include "../helpers/conversor.h"
#include "../coordinator/parser.h"

using namespace std;
// Macros
#define COORDINATOR_IP "127.0.0.1" //loopback address
#define COORDINATOR_PORT 30742

int connectWithCoordinator()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        throw "Err while opening socket connection";
    }

    sockaddr_in address;
    int addressSize = sizeof(sockaddr_in);
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    inet_pton(AF_INET, (const char *)COORDINATOR_IP, &address.sin_addr);

    int connStatus = connect(sock, (const sockaddr *)&address, (socklen_t)addressSize);
    if (connStatus != 0)
    {
        throw "Error while connecting to coordinator";
    }
    cout << getpid() << " is connected with coordinator" << endl;
    return sock;
}

char *generateMessage(int pid, int msgType)
{
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 0" << endl;
    // char msg[MESSAGE_MAX_SIZE];
    char *msg = new char[MESSAGE_MAX_SIZE];
    for (int j = 0; j < MESSAGE_MAX_SIZE; j++)
        msg[j] = '0';

    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 1" << endl;
    cout << "Message to send is " << msg << endl;

    msg[MESSAGE_MAX_SIZE] = '\0';

    char strId[MESSAGE_MAX_SIZE + 1];
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 2" << endl;
    itoa(pid, strId, 10);
    if (msgType == 1)
    {
        cout << "aaaaaaaaaaaa" << endl;
        msg[0] = '1';
    }
    else
    {
        cout << "kkkkkkkkkkkkk" << endl;
        msg[0] = '2';
    }
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 3" << endl;
    cout << "Message to send is " << msg << endl;
    msg[1] = SEPARATOR;
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 4" << endl;
    cout << "Message to send is " << msg << endl;

    int i = 0;
    while (strId[i] != '\0')
    {
        msg[i + 2] = strId[i];
        i++;
    }
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 5" << endl;
    cout << "Message to send is " << msg << endl;
    msg[i + 2] = SEPARATOR;
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 6" << endl;
    cout << "Message to send is " << msg << endl;

    // Filling with zeroes the rest of the message
    for (int j = i + 3; j < MESSAGE_MAX_SIZE; j++)
        msg[j] = '0';
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 7" << endl;
    cout << "Message to send is " << msg << endl;

    msg[MESSAGE_MAX_SIZE] = '\0';
    cout << "Will generate message of the type " << msgType << " for pid " << pid << " STEP 8" << endl;
    cout << "Message to send is " << msg << endl;

    return msg;
}

void request(int socketFd, int id)
{
    char *message = generateMessage(id, 1);
    cout << "--------------------------- " << message[0] << endl;
    cout << "Message sent is : " << message << endl;
    send(socketFd, message, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
    ssize_t sockStatus = recv(socketFd, message, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
    cout << "Response received: " << message << "-------" << endl;
    // cout << "Return" << sockStatus << endl;
    // if (sockStatus == -1)
    // {
    //     throw "Error on socket connection";
    // }
    cout << message[0] << endl;
    if (message[0] == '3')
    {
        cout << "Received GRANT " << endl;
        return;
    }
    else
    {
        throw "Request DENIED";
    };
};

void write(int processId)
{
    ofstream file;
    string timeStamp = getTimestamp();
    file.open("resultado.csv", ofstream::out | ofstream::app);
    file << timeStamp + "," << processId << endl;
    file.close();
}

void release(int socketFd, int id)
{
    char *message = generateMessage(id, 2);
    cout << "Message sent is : " << message << endl;
    send(socketFd, message, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
}

int main(int argc, char **argv)
{

    cout << argv[1] << endl;
    cout << argv[2] << endl;
    cout << argv[3] << endl;
    if (argc != 4)
    {
        cerr << "The program expects the following command line argument: n, r, k" << endl;
        return 0;
    }
    int n = atoi(argv[1]); // número de processos
    int r = atoi(argv[2]); // número de repetições
    int k = atoi(argv[3]); // segundos a aguardar no sleep

    int pids[n];

    ofstream file;
    file.open("resultado.csv", ofstream::out | ofstream::app);
    file << "timeStamp"
         << ","
         << "PID" << endl;
    file.close();

    // mutex writeMutex;
    for (int i = 0; i < n; i++)
    {
        // Creates a new copy of the process
        if ((pids[i] = fork()) < 0)
        {
            throw "Error while forking process";
        }
        else if (pids[i] == 0)
        {
            int processId = getpid();
            int sock = connectWithCoordinator();

            // cout << "Generated process" << processId << endl;
            string timeStamp;
            // ofstream file;
            for (int j = 0; j < r; j++)
            {
                cout << processId << " Will request coordinator  0" << endl;
                request(sock, processId);
                cout << processId << " Will write into csv file 1" << endl;
                cout << processId << " Will write into csv file 2" << endl;
                // timeStamp = getTimestamp();
                cout << processId << " Will write into csv file 3" << endl;
                cout << processId << " Will write into csv file 4" << endl;
                write(processId);
                // file << "xxxx"
                //      << ","
                //      << "PID" << endl;

                // cout << processId << " Will write into csv file 5" << endl;
                // cout << processId << " Will write into csv file 6" << endl;

                cout << processId << " Will sleep for " << k << "seconds" << endl;
                sleep(k);
                cout << processId << " Will send release message" << endl;
                release(sock, processId);
            }
            close(sock);
        }
    }
    // file.close();
}