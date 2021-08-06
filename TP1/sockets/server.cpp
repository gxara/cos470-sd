#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 3000
using namespace std;

int generateRandomNumbers(int ant)
{
    return rand() % 100 + ant;
}

void socketConection()
{
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Insira a quantidade de números a ser gerado" << endl;
        return 0;
    }
    int cont = 0, limit = stoi(argv[1]), randomNumber = 0;
    if (limit <= 0)
    {
        cout << "Insira um limite positivo e não nulo.";
    }

    srand(time(NULL));
    int server, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024];
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == 0)
    {
        perror("Falha na criação do socket!");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Erro ao vincular porta 3000");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server, 3) < 0)
    {
        perror("erro no listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    do
    {
        randomNumber = generateRandomNumbers(randomNumber);
        cont++;
        const char *msg = (to_string(randomNumber)).c_str();
        // envia pro cliente.
        send(new_socket, msg, strlen(msg), 0);
        valread = read(new_socket, buffer, 1024);
        int answer = atoi(buffer);
        if (answer == 1)
        {
            cout << "Enviei o: " << randomNumber << " e recebi que ele é primo" << endl;
        }
        else
        {
            cout << "Enviei o: " << randomNumber << " e recebi que ele não é primo" << endl;
        }
    } while (cont < limit);

    cout << "Enviando zero para o consumidor. Encerrando!" << endl;
    send(new_socket, "0", strlen("0"), 0);
    return 0;
}
