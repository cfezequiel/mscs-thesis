#include "ariarobot.h"

AriaRobot::AriaRobot()
{
    // do nothing
}

void AriaRobot::connect(string username, string password, string server)
{
    Aria::init();

    client = new ArClientBase;

    ArArgumentBuilder builder;
    builder.addPlain("-host");
    builder.addPlain(server.c_str());
    builder.addPlain("-user");
    builder.addPlain(username.c_str());
    builder.addPlain("-password");
    builder.addPlain(password.c_str());

    ArArgumentParser parser(&builder);

    ArClientSimpleConnector clientConnector(&parser);

    // Connect Aria client to remote server
    if (!clientConnector.connectClient(client))
    {
        if (client->wasRejected())
        {
            printf("Server '%s' rejected connection, exiting\n",
                   client->getHost());
        }
        else
        {
            printf("Could not connect to server '%s', exiting\n",
                   client->getHost());
        }
        exit(1);
    }

    // Set the robot name as server host name
    client->setRobotName(client->getHost());

    // Run the client in a background thread
    client->runAsync();

    // Client will stop running when disconnected with server,
    // or Aria is shutdown
    client->disconnect();
    Aria::shutdown();

    return;
}
