#ifndef ROBOTCLIENT_H
#define ROBOTCLIENT_H

#include <string>
#include "Aria.h"
#include "ArNetworking.h"
using namespace std;

// Class to connect to an Adept MobileRobots robot server,
// based on the Advanced Robot Interface for Applications (ARIA) library
class RobotClient
{
public:
    RobotClient();
    ~RobotClient();
    void connect(string username, string password, string server);
    void disconnect();

private:
    ArClientBase *client;
};

#endif // ROBOTCLIENT_H
