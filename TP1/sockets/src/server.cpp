#include "../../helpers/isPrime.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>

#define PORT 3000
using namespace std;

int createSocket()
{
    cout << "Will create socket \n";
    int mySocket = socket(
        AF_INET,     // IPV4 Family
        SOCK_STREAM, // SOCK_STREAM for TCP and SOCK_DGRAM for UDP
        0            //Protocol code... 0 for Internet Protocol(IP)
    );

    return mySocket;
};

int main(int argc, char **argv)
{
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    int valread;
    char buffer[1024] = {0};
    int myFirstCppSocket;
    int myFirstCppSocketFd = createSocket();
    if (myFirstCppSocketFd < 0)
    {
        cout << "\n Socket creation error \n";
        return -1;
    };

    cout << "Socket created" << endl;

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET,
                  "127.0.0.1",
                  &serv_addr.sin_addr) <= 0)
    {
        cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (bind(myFirstCppSocketFd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(myFirstCppSocketFd, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((myFirstCppSocket = accept(myFirstCppSocketFd, (struct sockaddr *)&serv_addr,
                                   (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    read(myFirstCppSocket, buffer, 1024);
    int requestedNumber = 1;
    while (requestedNumber > 0)
    {
        valread = read(myFirstCppSocketFd, buffer, 1024);
        int requestedNumber = atoi(buffer);

        cout << "Received number " << requestedNumber << endl;

        memset(buffer, 0, sizeof(buffer));

        cout << "Checking if number " << requestedNumber << " is prime " << endl;
        bool prime = int(isPrime(requestedNumber)); // 1 if is prime, 0 if not
        const char *response = (to_string(prime)).c_str();

        cout << response << endl;

        cout << "Will send response...  " << endl;
        send(myFirstCppSocket, response, strlen(response), 0);
    }
}