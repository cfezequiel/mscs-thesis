#include "sessiondialog.h"
#include "ui_sessiondialog.h"

SessionDialog::SessionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionDialog)
{
    ui->setupUi(this);
}

SessionDialog::~SessionDialog()
{
    delete ui;
}

void SessionDialog::on_okButton_clicked()
{
    // Get session name
    close();
}

QString SessionDialog::sessionName()
{
    return ui->sessionNameLineEdit->text();
}

QString SessionDialog::getSessionName()
{
    SessionDialog dialog;
    dialog.exec();
    return dialog.sessionName();
}
