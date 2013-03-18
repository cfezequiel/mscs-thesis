#ifndef SESSIONDIALOG_H
#define SESSIONDIALOG_H

#include <QDialog>

namespace Ui {
class SessionDialog;
}

class SessionDialog : public QDialog
{
    Q_OBJECT
    
public:
    static QString getSessionName();
    QString sessionName();
    explicit SessionDialog(QWidget *parent = 0);
    ~SessionDialog();
    
private slots:
    void on_okButton_clicked();

private:
    Ui::SessionDialog *ui;
    QString _sessionName;
};

#endif // SESSIONDIALOG_H
