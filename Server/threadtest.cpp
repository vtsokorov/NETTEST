#include "threadtest.h"
#include <QFile>
#include <QTextStream>

const int readBlock = 5;

ThreadTest::ThreadTest(int socketDescriptor, QObject *parent)
          : QThread(parent), m_socketDescriptor(socketDescriptor), m_stopped(false)
{

}

void ThreadTest::signalsConnection()
{
    connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SIGNAL(error(QAbstractSocket::SocketError)), Qt::DirectConnection);

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displayError(QAbstractSocket::SocketError)), Qt::DirectConnection);

    connect(m_client, SIGNAL(disconnected()), this,
            SLOT(onDisconnect()), Qt::DirectConnection);
}

void ThreadTest::run()
{
    int evaluation = 0;
    m_client = new QTcpSocket();
    if (!m_client->setSocketDescriptor(m_socketDescriptor)) {
        emit error(m_client->error());
        return;
    }

    signalsConnection();

    m_addr = m_client->peerAddress();

    emit printString(QObject::tr("Поток %1: Подключился новый клиент с адреса %2")
                         .arg(m_socketDescriptor).arg(m_addr.toString()));

    while(!m_client->waitForReadyRead(100)){
       if(m_stopped) return;
    }

    QByteArray userName = m_client->readAll();
    QString name(userName);

    emit printString(QObject::tr("Поток %1: Имя пользователя %2")
                         .arg(m_socketDescriptor).arg(name));

    QFile file("test.txt");
    if(!file.exists()) {
        emit printString(QObject::tr("Отсутствует файл данных"));
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit printString(QObject::tr("Невозможно открыть файл данных"));
        return;
    }

    while(!file.atEnd())
    {
        int count = 0; QString answer;
        while (count < readBlock)
        {
            QString data = QString::fromLocal8Bit(file.readLine()).trimmed();
            if(!data.isEmpty())
            {
                if(data[0] == '+') {
                    data = data.right(data.length()-1);
                    answer = data.right(data.length()-1);
                }
                writeSocketFrom(data.toUtf8());
            }
            else continue;
            count++;
        }

        while(!m_client->waitForReadyRead(100)){
           if(m_stopped) return;
        }

        QByteArray select = m_client->readAll();
        QString clientAnswer(select);

        emit printString(QObject::tr("Поток %1: Получены данные от клиента %2: %3")
                            .arg(m_socketDescriptor).arg(m_addr.toString()).arg(clientAnswer));

        if(answer == clientAnswer)
            evaluation++;

        if(m_stopped) return;
    }

    file.close();

    QFile res("result.txt");
    if(res.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QTextStream out(&res);
        out.setCodec("UTF-8");
        out << tr("Результат пользователя : ") << name << " - "<< evaluation <<"\n";
        res.close();
        emit printString(QObject::tr("Поток %1: Результат сохранен в файл")
                         .arg(m_socketDescriptor));
    }
    else
    {
        emit printString(QObject::tr("Невозможно открыть файл результатов"));
        return;
    }

    QString endBytes = "EOF-" + QString::number(evaluation);
    m_client->write(QByteArray(endBytes.toUtf8()));

    exec();
}

int ThreadTest::writeSocketFrom(const QByteArray &message)
{
    if (m_client->write(message) <= 0)
    {
        emit printString(tr("Невозможно записать данные: %1")
                .arg(m_client->errorString()));
        return -1;
    }
    return 0;
}

void ThreadTest::onDisconnect()
{
    m_client->deleteLater();

    emit printString(QObject::tr("Поток %1: остановлен").arg(m_socketDescriptor));

    quit();
}

void ThreadTest::displayError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError)
        emit printString(QObject::tr("Поток %1: Клиент отключился.")
                            .arg(m_socketDescriptor));
    else if(error != QAbstractSocket::SocketTimeoutError)
        emit printString(QObject::tr("Ошибка : %1")
                         .arg(m_client->errorString()));
}
