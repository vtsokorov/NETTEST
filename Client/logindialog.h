#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QtNetwork>

#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:

    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QTcpSocket *getSocket() { return &socket; }
    QString name();

private:

    Ui::LoginDialog *ui;
    QDialogResult result;
    QTcpSocket socket;

    int writeSocketFrom(const QByteArray &message);

public slots:

    QDialogResult showDialog();
    void acceptMethod();
    void cancelMethod();
};

#endif // LOGINDIALOG_H
