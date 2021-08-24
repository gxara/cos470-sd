#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

#define N 1            /*número de espaços no buffer*/
typedef int semaphore; /*inicializando o semáforo*/
semaphore mutex = 1;   /*controla o acesso à região crítica*/
semaphore empty = N;   /*contador do número de espaços vazios no buffer*/
semaphore full = 0;    /*contador do número de espaços cheios no buffer*/
int buffer[N];

int posicaoInsercao = 0;
int posicaoLeitura = 0;
int contador = 0;

int MAX = 100000;

int Np = 1;
int Nc = 1;

bool isPrime(int number)
{
    for (int i = 2; i <= number / 2; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

int generateRandomNumber(int base = 0)
{
    int number = rand() % 10000000 + 1;
    if (number <= base)
    {
        number += base;
    }
    return number;
}

void writeBuffer(int valor)
{
    buffer[posicaoInsercao] = valor;
    posicaoInsercao++;
    if (posicaoInsercao == N)
        posicaoInsercao = 0;
    contador++;
}

int readBuffer()
{
    int tmp = buffer[posicaoLeitura];
    posicaoLeitura++;
    if (posicaoLeitura == N)
        posicaoLeitura = 0;
    return tmp;
}

void wait(semaphore semaphore)
{
    semaphore--;
    if (semaphore == 0)
    {
        std::chrono::milliseconds(200);
        wait(semaphore);
    }
}

void signal(semaphore semaphore)
{
    semaphore++;
}

void producer(void)
{
    int M = 100000;
    int item;
    while (M > contador)
    {
        item = generateRandomNumber();
        while (true)
        {
            wait(empty);
            wait(mutex);
            writeBuffer(item);
            signal(mutex);
            signal(full);
        }
    }
}

void consumer(void)
{
    int item;
    bool prime;
    while (true)
    {
        wait(full);
        wait(mutex);
        item = readBuffer();
        signal(mutex);
        signal(empty);
        prime = isPrime(item);
        printf("%d\n", prime);
    }
}

int main(int argc, char *argv[])
{
    using namespace std;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    Np = atoi(argv[1]);
    Nc = atoi(argv[2]);
    MAX = atoi(argv[3]);

    std::vector<thread> producers_and_consumers;

    // Create producers
    for (int i = 0; i < Np; ++i)
    {
        producers_and_consumers.push_back(thread(producer, i));
    }

    // Create consumers
    for (int i = 0; i < Nc; ++i)
    {
        producers_and_consumers.push_back(thread(consumer, i));
    }

    // Join consumers and producers
    for (auto &t : producers_and_consumers)
    {
        t.join();
    }

    end = std::chrono::system_clock::now();

    long elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    return 0;
}
