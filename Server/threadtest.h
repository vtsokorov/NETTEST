#ifndef THREADTEST_H
#define THREADTEST_H

#include <QObject>
#include <QThread>
#include <QtNetwork>


class ThreadTest : public QThread
{
    Q_OBJECT

public:

    ThreadTest(int socketDescriptor, QObject *parent = 0);

protected:

    void run();

private:

    int m_socketDescriptor;
    QTcpSocket *m_client;
    QHostAddress m_addr;
    volatile bool m_stopped;

    void signalsConnection();
    int writeSocketFrom(const QByteArray &message);

public slots:

    void stop() { m_stopped = true; }
    void onDisconnect();
    void displayError(QAbstractSocket::SocketError error);

signals:

    void error(QAbstractSocket::SocketError);
    void printString(QString);
};

#endif // ECHOSERVER_H
