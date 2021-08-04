#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <random>

using namespace std;

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

void producer(int interations, int file)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);

    int number = 1;
    FILE *stream;
    stream = fdopen(file, "w");
    if (stream == NULL)
    {
        cout << "ERROR opening pipe." << endl;
    }
    for (int i = 0; i < interations; i++)
    {
        number += dis(gen);
        fprintf(stream, "%i \n", number);
    }
    fprintf(stream, "%i \n", 0);
    fclose(stream);
}

void consumer(int file)
{
    int number, recived;
    FILE *stream;
    stream = fdopen(file, "r");
    if (stream == NULL)
        if (stream == NULL)
        {
            cout << "ERROR opening pipe." << endl;
        }
    while (true)
    {
        recived = fscanf(stream, "%i \n", &number);
        if (recived < 0)
        {
            cout << "ERROR reading from pipe." << endl;
        }
        if (number == 0)
        {
            break;
        }
        if (isPrime(number))
        {
            printf("%i is prime\n", number);
        }
        else
        {
            printf("%i is NOT prime\n", number);
        }
    }
    fclose(stream);
}

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        cout << "The program expects the following command line arguments: N (number of messages to send throught the pipe)" << endl;
        return 0;
    }

    int N = atoi(argv[1]);

    pid_t child_process_pid;
    int fd[2]; // fd(file descriptor) for the read and write operations of the pipe.

    if (pipe(fd) < 0)
    {
        cout << "Error on pipe creation" << endl;
        exit(-1);
    }

    child_process_pid = fork();
    if (child_process_pid < (pid_t)0)
    {
        cout << "Fork failed." << endl;
    }

    if (child_process_pid == (pid_t)0)
    {
        // This is the child process.
        close(fd[1]);
        try
        {
            consumer(fd[0]);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
        }

        return 0;
    }
    else
    {
        // This is the parent process. Close other end first
        close(fd[0]);
        try
        {
            producer(N, fd[1]);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
        }

        return 0;
    }
}