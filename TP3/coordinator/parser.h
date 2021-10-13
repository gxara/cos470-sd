#include <iostream>

#define MESSAGE_MAX_SIZE 10
#define MESSAGE_REQUEST 1
#define MESSAGE_RELEASE 2
#define MESSAGE_GRANT 3
#define SEPARATOR '|'

struct message
{
    int id;
    int fd;
    int type;
};
message parseMessage(char *buffer, int fd);
