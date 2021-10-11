#include <iostream>

#define MESSAGE_MAX_SIZE 10
#define MESSAGE_REQUEST 1
#define MESSAGE_GRANT 2
#define MESSAGE_RELEASE 3
#define SEPARATOR '.'

struct message
{
    int id;
    int fd;
    int type;
};

message parseMessage(char buffer[MESSAGE_MAX_SIZE + 1], int fd)
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
    ret.fd = fd;
    ret.type = messageType;
    ret.id = atoi(tmp);
    return ret;
}