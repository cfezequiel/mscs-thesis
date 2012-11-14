#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QtGui>
#include <QDialog>

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = 0);
    QString getUsername() const;
    QString getPassword() const;
    QString getServer() const;
    
signals:
    
public slots:

private slots:
    void storeInfo();

private:
    QPushButton *cancelButton;
    QPushButton *connectButton;
    QComboBox *usernameComboBox;
    QComboBox *passwordComboBox;
    QComboBox *serverComboBox;
    QString username;
    QString password;
    QString server;
};

#endif // CONNECTDIALOG_H




