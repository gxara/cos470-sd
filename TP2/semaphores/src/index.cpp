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
#include "../../helpers/isPrime.h"

using namespace std;
#define N 32 /*número de espaços no buffer*/

typedef int semaphore;          /*inicializando o semáforo*/
semaphore read_write_mutex = 1; /*controla o acesso à região crítica*/
semaphore emptySemaphore = N;   /*contador do número de espaços vazios no buffer*/
semaphore full = 0;             /*contador do número de espaços cheios no buffer*/
int buffer[N];

int writePosition = 0;
int readPosition = 0;
int numbersConsumed = 0;

int MAX = 1000;

int Np = 1;
int Nc = 1;

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
    buffer[writePosition] = valor;
    writePosition++;
    if (writePosition == N)
        writePosition = 0;
}

int readBuffer()
{
    int tmp = buffer[readPosition];
    numbersConsumed++;
    readPosition++;
    if (readPosition == N)
        readPosition = 0;
    return tmp;
}

void wait(semaphore semaphore)
{
    semaphore--;
    if (semaphore == 0)
    {
        chrono::milliseconds(200);
        wait(semaphore);
    }
}

void signal(semaphore semaphore)
{
    semaphore++;
}

void producer(void)
{
    int item;
    while (MAX > numbersConsumed)
    {
        item = generateRandomNumber();
        wait(emptySemaphore);
        wait(read_write_mutex);
        writeBuffer(item);
        signal(read_write_mutex);
        signal(full);
    }
}

void consumer(void)
{
    int item;
    bool prime;

    while (MAX > numbersConsumed)
    {
        wait(full);
        wait(read_write_mutex);
        item = readBuffer();
        signal(read_write_mutex);
        signal(emptySemaphore);
        prime = isPrime(item);
        cout << prime << endl;
    }
}

int main(int argc, char *argv[])
{
    using namespace std;

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    if (argc != 3)
    {
        cout << "The program expects the following command line arguments: producers, consumers" << endl;
        return 0;
    }

    Np = atoi(argv[1]);
    Nc = atoi(argv[2]);
    double totalTime = 0;

    for (int iteration = 1; iteration <= 10; iteration++)
    {
        vector<thread> producers_and_consumers;

        // Create producers
        for (int i = 0; i < Np; ++i)
        {
            producers_and_consumers.push_back(thread(producer));
        }

        // Create consumers
        for (int i = 0; i < Nc; ++i)
        {
            producers_and_consumers.push_back(thread(consumer));
        }

        // Join consumers and producers
        for (auto &t : producers_and_consumers)
        {
            t.join();
        }

        end = chrono::system_clock::now();

        chrono::duration<double> elapsed = end - start;
        totalTime = totalTime + elapsed.count();

        cout << "\n************ ITERATION " << iteration << " ************\n"
             << endl;
        cout << "Consumer Threads: " << Nc << endl;
        cout << "Producer Threads: " << Np << endl;
        cout << "Execution time: " << elapsed.count() << endl;
        cout << "********************************\n"
             << endl;
    }

    cout << "\n************ RESULTADO FINAL ************\n"
         << endl;
    cout << "Consumer Threads: " << Nc << endl;
    cout << "Producer Threads: " << Np << endl;
    cout << "Average execution time: " << totalTime / 10 << endl;
    cout << "********************************\n"
         << endl;

    return 0;
}
