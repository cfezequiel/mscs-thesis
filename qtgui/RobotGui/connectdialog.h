#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <string>

#include <QPushButton>
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
    void checkText(const QString & text);

private:
    Ui::ConnectDialog *ui;
    QString _loginInfoFileName;
    QDomDocument *_loginInfo;
    QPushButton *_okButton;
};

#endif // CONNECTDIALOG_H
