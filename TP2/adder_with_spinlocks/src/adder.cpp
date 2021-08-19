
#include <iostream>
#include "../../helpers/randomNumbers.h"
using namespace std;

int populate(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = generateRandomNumber(100);
    }
    return array
}

int *splitIntoN(int *numbers, int chunks)
{
}

int main(int argc, char const *argv[])
{

    if (argc != 3)
    {
        cout << "The program expects the following command line argument: " << endl;
        cout << "number_of_elements: " << endl;
        cout << "number_of_threads: " << endl;
        return 0;
    }

    cout << generateRandomNumber(100) << endl;
    cout << generateRandomNumber(10) << endl;
    cout << generateRandomNumber(100) << endl;
    cout << generateRandomNumber(100) << endl;
    cout << generateRandomNumber(100) << endl;
}