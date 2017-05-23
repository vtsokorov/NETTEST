#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QtNetwork>
#include "global.h"

class QRadioButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QTcpSocket *tcp, QWidget *parent = 0);
    ~MainWindow();

    QDialogResult windowStatus();

protected:

    virtual void closeEvent(QCloseEvent *);

private:

    Ui::MainWindow *ui;

    QDialogResult result;
    QRadioButton *radio;
    QTcpSocket *socket;

    QByteArray block;
    QString evaluation;

    void init();
    int writeSocketFrom(const QByteArray &message);

private slots:

    void exitWindow();
    void closeWindow();
    void showAboutDialog();

    void select();
    void enter();

    bool nextQuestion();
    void clearSelect();
};

#endif // MAINWINDOW_H
