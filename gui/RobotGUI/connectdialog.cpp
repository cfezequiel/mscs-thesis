#include "connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    // Create labels
    QLabel *usernameLabel = new QLabel(tr("Username:"));
    QLabel *passwordLabel = new QLabel(tr("Password:"));
    QLabel *serverLabel = new QLabel(tr("Robot Server:"));

    // Create buttons
    cancelButton = new QPushButton(tr("Cancel"));
    connectButton = new QPushButton(tr("&Connect"));

    // Create fields
    usernameComboBox = new QComboBox;
    usernameComboBox->setEditable(true);
    passwordComboBox = new QComboBox;
    passwordComboBox->setEditable(true);
    serverComboBox = new QComboBox;
    serverComboBox->setEditable(true);

    // Add widgets to layout
    QFormLayout *layout = new QFormLayout;
    layout->addRow(usernameLabel, usernameComboBox);
    layout->addRow(passwordLabel, passwordComboBox);
    layout->addRow(serverLabel, serverComboBox);
    layout->addRow(cancelButton, connectButton);

    // Set the layout
    setLayout(layout);
    setWindowTitle(tr("Connect to robot"));

    // Connect signals to slots
    connect(connectButton, SIGNAL(clicked()), this, SLOT(storeInfo()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void ConnectDialog::storeInfo()
{
    username = usernameComboBox->currentText();
    password = passwordComboBox->currentText();
    server = serverComboBox->currentText();
}

QString ConnectDialog::getUsername() const
{
    return username;
}

QString ConnectDialog::getPassword() const
{
    return password;
}

QString ConnectDialog::getServer() const
{
    return server;
}
