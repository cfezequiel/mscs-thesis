#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <sstream>
#include <iostream>

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDomElement>
#include <QDialogButtonBox>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    // Load previously generated login info
    _loginInfoFileName.append("logininfo.xml");
    _loginInfo = new QDomDocument("logininfo");
    QFile file(_loginInfoFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        _loginInfo->setContent(&file);
    }
    file.close();

    if (!_loginInfo->hasChildNodes())
    {
        _loginInfo->appendChild(_loginInfo->createElement("info"));
        return;
    }

    // Fill combo boxes with login info
    QDomNode root = _loginInfo->firstChild();
    for (QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        QDomElement e = node.toElement();
        QString host = e.attribute("host");
        QString port = e.attribute("port");
        ui->hostComboBox->addItem(host);
        ui->portComboBox->addItem(port);
    }
    // Set the host/port items in the comboboxes to the last one generated
    // in the file
    int lastIndex = ui->hostComboBox->count() - 1;
    ui->hostComboBox->setCurrentIndex(lastIndex);
    ui->portComboBox->setCurrentIndex(lastIndex);


    // Disable OK button until user inputs necessary information
    _okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    _okButton->setEnabled(false);

    QObject::connect(ui->usernameLineEdit, SIGNAL(textEdited(const QString &)),
                    this, SLOT(checkText(const QString &)));
    QObject::connect(ui->hostComboBox, SIGNAL(editTextChanged(const QString &)),
                    this, SLOT(checkText(const QString &)));
    QObject::connect(ui->portComboBox, SIGNAL(editTextChanged(const QString &)),
                    this, SLOT(checkText(const QString &)));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
    delete _loginInfo;
    delete _okButton;
}

void ConnectDialog::on_buttonBox_accepted()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString host = ui->hostComboBox->lineEdit()->text();
    QString port = ui->portComboBox->lineEdit()->text();

    // Send signal to connect passing the login info
    connect(host, port.toInt(), username, password);

    // Check for duplicates
    QDomNode root = _loginInfo->firstChild();
    for (QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        QDomElement e = node.toElement();
        if (host == e.attribute("host") && port == e.attribute("port"))
        {
            return;
        }
    }

    // Save login info
    QDomElement login = _loginInfo->createElement("login");
    login.setAttribute(QString("host"), host);
    login.setAttribute(QString("port"), port);
    root.appendChild(login);

    QFile file(_loginInfoFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << _loginInfo->toString();
    }
    file.close();

    // Add login info to comboboxes
    ui->hostComboBox->addItem(host);
    ui->portComboBox->addItem(port);
}

void ConnectDialog::checkText(const QString & text)
{
    if (!ui->usernameLineEdit->text().isEmpty() &&
        !ui->hostComboBox->currentText().isEmpty() &&
        !ui->portComboBox->currentText().isEmpty())
    {
        _okButton->setEnabled(true);
    }
    else
    {
        _okButton->setEnabled(false);
    }
}
