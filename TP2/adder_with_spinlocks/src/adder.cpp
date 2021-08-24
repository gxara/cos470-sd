
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include "../../helpers/randomNumbers.h"
#include "../../helpers/splitVector.h"
using namespace std;

#define NUM_ELEMENTS 100

atomic_flag lock_stream = ATOMIC_FLAG_INIT;

int cumulativeSum = 0;

void acquire(atomic_flag *lock)
{
    while (lock->test_and_set())
        ;
}

void release(atomic_flag *lock)
{
    lock->clear();
}

void acumulate(vector<int> chunk, int threadNum)
{
    acquire(&lock_stream);
    cout << "[THREAD " << threadNum << "] Will sum data inside chunk " << threadNum << endl;
    int chunkSum = accumulate(chunk.begin(), chunk.end(), 0);
    cout << "[THREAD " << threadNum << "] Chunk sum = " << chunkSum << endl;
    cumulativeSum += chunkSum;
    release(&lock_stream);
}

vector<int> populate(int size)
{
    vector<int> array;
    for (int i = 0; i < size; i++)
    {
        int num = (rand() % 201) - 100;
        array.push_back(num);
    }
    return array;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Threads quantity\n";
        return 0;
    }

    vector<int> numbers = populate(NUM_ELEMENTS);
    int numberOfThreads = atoi(argv[1]);
    int chunkSize = NUM_ELEMENTS / numberOfThreads;

    vector<vector<int>> vectorOfChunks = splitVectorIntoChunks(numbers, chunkSize);

    vector<thread> v;

    // Starting to measure time
    auto start = chrono::system_clock::now();

    for (int i = 0; i < numberOfThreads; i++)
    {
        vector<int> chunk = vectorOfChunks[i];

        // https://www.cplusplus.com/reference/thread/thread/
        thread thread_n(acumulate, chunk, i);
        v.push_back((thread &&)(thread_n));
    }

    for (int i = 0; i < numberOfThreads; i++)
    {
        // Must wait for each thread to finish
        v[i].join();
    }

    // Ending to measure time
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "\n************ RESULT ************" << endl;
    cout << "Cumulative Sum: " << cumulativeSum << endl;
    cout << "Threads Quantity: " << numberOfThreads << endl;
    cout << "Execution time: " << elapsed.count() << endl;
    cout << "********************************\n"
         << endl;

    return 1;
}