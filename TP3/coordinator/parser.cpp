#include <iostream>
#include "parser.h"

message parseMessage(char *buffer, int fd)
{
    message ret;
    int messageType, i = 2;
    messageType = (int)buffer[0] - '0';
    char tmp[MESSAGE_MAX_SIZE - 1];
    while ((buffer[i] != SEPARATOR) && (buffer[i] != '\0'))
    {
        tmp[i - 2] = buffer[i];
        i++;
    }
    tmp[i - 2] = '\0';
    std::cout << "-- Message received from PID " << tmp << " with type " << messageType << std::endl;
    ret.fd = fd;
    ret.type = messageType;
    ret.id = atoi(tmp);
    return ret;
}