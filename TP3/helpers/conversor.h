#include <iostream>

using namespace std;

/* A utility function to reverse a string  */
// Retirado de: https://www.geeksforgeeks.org/implement-itoa/#:~:text=For%20example%3A%2D%20if%20base,with%20a%20minus%20sign%20(%2D).
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        swap(*(str + start), *(str + end));
        start++;
        end--;
    }
}

// Implementation of itoa()
// Retirado de: https://www.geeksforgeeks.org/implement-itoa/#:~:text=For%20example%3A%2D%20if%20base,with%20a%20minus%20sign%20(%2D).
char *itoa(int num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}