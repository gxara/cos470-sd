#include <iostream>

int generateRandomNumber(int base = 0)
{
    // srand(time(NULL));
    int number = rand() % 100 + 1;
    if (number <= base)
    {
        number += base;
    }
    return number;
}