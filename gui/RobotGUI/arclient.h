#ifndef ARCLIENT_H
#define ARCLIENT_H

#include "Aria.h"
#include "ArNetworking.h"

class ArClient : public ArClientBase
{
public:
    ArClient();
    bool connect(char *host, int port, char *username, char *password);
    void getRobotInfo();
};

#endif // ARCLIENT_H
