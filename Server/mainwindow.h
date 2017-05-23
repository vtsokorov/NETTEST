#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "servertest.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVisible(bool visible);

protected:

    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private:

    void createActions();
    bool closQuestione();

    Ui::MainWindow *ui;
    Qt::WindowFlags flags;

    QSystemTrayIcon *trayIcon;

    QMenu *trayIconMenu;
    QAction *aboutAction;
    QAction *restoreAction;
    QAction *exitAction;

    ServerTest server;

public slots:

    void showMinimized();
    void showNormal();
    void close();

private slots:

    void startServer();
    void stopServer();

    void printMessage(const QString& message);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showAboutDialog();

signals:

    void closeSignal();
};

#endif // MAINWINDOW_H
