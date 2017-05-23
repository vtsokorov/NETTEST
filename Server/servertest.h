#ifndef SERVERTEST_H
#define SERVERTEST_H

#include <QObject>
#include <QtNetwork>
#include <QPointer>
#include "threadtest.h"

class ServerTest : public QTcpServer
{
    Q_OBJECT

public:

    ServerTest(QObject *parent = 0);
    bool isRuning() {return isRun;}

protected:

    void incomingConnection(int socketDescriptor);
    QList<QPointer<ThreadTest>> m_threads;

private:

    bool isRun;

public slots:

    bool start();
    void stop();
    void removeThread();

signals:

    void printString(QString);
};

#endif // SERVERTEST_H
