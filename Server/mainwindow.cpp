#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutdialog.h>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    flags = windowFlags() ^ Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);

    createActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::closQuestione()
{
    int r = QMessageBox::question(this, tr("Подтвердите"), tr("Выйти из программы?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    return (r == QMessageBox::Yes);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(closQuestione()){
        if(server.isRuning())
            server.stop();
        qApp->quit();
    }
    else
        event->ignore();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        if (windowState() == Qt::WindowMinimized)
        {
            event->ignore();
            trayIcon->show();
            trayIcon->eventFilter(this, event);
            hide();
            return;
        }
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::setVisible(bool visible)
{
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::createActions()
{
    connect(ui->act_start, SIGNAL(triggered()),this, SLOT(startServer()));
    connect(ui->act_stop, SIGNAL(triggered()),this, SLOT(stopServer()));
    connect(ui->act_exit, SIGNAL(triggered()),this, SLOT(close()));

    connect(ui->act_about, SIGNAL(triggered()),this, SLOT(showAboutDialog()));

    aboutAction = new QAction(tr("О программе"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    restoreAction = new QAction(tr("&Отобразить окно"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    exitAction = new QAction(tr("&Закрыть сервер"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    connect(&server, SIGNAL(printString(QString)), this, SLOT(printMessage(QString)));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/img/server.png"));
    trayIcon->setToolTip("Сервер тестирования");
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::close()
{
    if(closQuestione()){
        if(server.isRuning())
            server.stop();
        qApp->quit();
    }
}

void MainWindow::showMinimized()
{
    trayIcon->show();
    hide();

    if(!server.isRuning()){
        server.start();
        ui->act_start->setDisabled(server.isRuning());
    }
}

void MainWindow::showNormal()
{
    trayIcon->hide();
    QMainWindow::showNormal();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) {
        this->showNormal();
        this->activateWindow();
    }
}

void MainWindow::startServer()
{
   server.start();

   ui->act_start->setDisabled(true);
   ui->act_stop->setDisabled(false);
}

void MainWindow::stopServer()
{
   server.stop();

   ui->act_start->setDisabled(false);
   ui->act_stop->setDisabled(true);
}

void MainWindow::printMessage(const QString& message)
{
    ui->lw_client_list->addItem(message);

    if(trayIcon->isVisible())
    {
        QSystemTrayIcon::MessageIcon icon =
                QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("Сообщение сервера", message, icon, 9000);
    }
}

void MainWindow::showAboutDialog()
{
    AboutDialog dlg;
    dlg.exec();
}


