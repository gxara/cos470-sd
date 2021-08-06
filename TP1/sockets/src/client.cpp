// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#define PORT 3000
using namespace std;

int generateRandomNumbers(int base = 0)
{
    srand(time(NULL));
    int number = rand() % 100 + 1;
    if (number <= base)
    {
        number += base;
    }
    return number;
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread, randomNumber;
    int numbersGenerated = 0;
    int desiredNumbers = atoi(argv[1]);

    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    do
    {
        randomNumber = generateRandomNumbers(randomNumber);
        numbersGenerated++;
        const char *msg = (to_string(randomNumber)).c_str();
        send(sock, msg, strlen(msg), 0);
        valread = read(sock, buffer, 1024);
        int answer = atoi(buffer);
        if (answer == 1)
        {
            cout << "Sending number: " << randomNumber << "... Received that this number is prime" << endl;
        }
        else
        {
            cout << "Sending number: " << randomNumber << "... Received that this number is NOT prime" << endl;
        }
    } while (numbersGenerated < desiredNumbers);

    send(sock, "0", strlen("0"), 0);
    cout << "Done ... " << endl;

    return 0;
}