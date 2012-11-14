#ifndef ARIAROBOT_H
#define ARIAROBOT_H

#include <string>
#include "Aria.h"
#include "ArNetworking.h"
using namespace std;

// Class to connect to an Adept MobileRobots robot server,
// based the Advanced Robot Interface for Applications (ARIA) library
class AriaRobot
{
public:
    AriaRobot();
    void connect(string username, string password, string server);

private:
    ArClientBase *client;
};

#endif // ARIAROBOT_H
