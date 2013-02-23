#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <string>

#include <QDialog>
#include <QDomDocument>

using namespace std;

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();
    
signals:
    void connect(QString host, int port, QString username, QString password);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ConnectDialog *ui;
    QString _loginInfoFileName;
    QDomDocument *_loginInfo;
};

#endif // CONNECTDIALOG_H
