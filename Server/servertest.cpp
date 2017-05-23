#include "servertest.h"

const int StopWait = 100;

ServerTest::ServerTest(QObject *parent) :
    QTcpServer(parent), isRun(false)
{

}

bool ServerTest::start()
{
    QHostAddress addr;
    if ( !listen(QHostAddress::Any, 5678) ) {
        emit printString(QObject::tr("Запуск сервера не возможен: %1").arg(errorString()));
        close();
        isRun = false;
        return isRun;
    }
    emit printString(QObject::tr("Сервер запущен"));
    isRun = true;
    return isRun;
}

void ServerTest::stop()
{
    foreach (ThreadTest *thread, m_threads){
        thread->stop();
        thread->exit(-1);
    }
    m_threads.clear();

//    while(m_threads.count())
//    {
//        QMutableListIterator<QPointer<ThreadTest>> i(m_threads);
//        while(i.hasNext()) {
//            ThreadTest *test = i.next();
//            if(test) {
////                test->disconnectFromHost();
//                if(test->wait(StopWait)) {
//                    delete test;
//                    i.remove();
//                }
//            }
//            else
//                i.remove();
//        }
//    }

    isRun = false;
    this->close();
    emit printString(QObject::tr("Сервер остановлен"));
}

void ServerTest::incomingConnection(int socketDescriptor)
{
    ThreadTest *thread = new ThreadTest(socketDescriptor, this);

    connect(thread, SIGNAL(finished()), this, SLOT(removeThread()),
            Qt::DirectConnection);

    connect(thread, SIGNAL(printString(QString)), this, SIGNAL(printString(QString)),
            Qt::DirectConnection);

    m_threads.append(thread);
    thread->start();
}

void ServerTest::removeThread()
{
    ThreadTest *thread = qobject_cast<ThreadTest*>(sender());
    m_threads.removeAt(m_threads.indexOf(thread));
    thread->deleteLater();
}

