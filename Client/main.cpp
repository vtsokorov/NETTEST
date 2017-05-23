#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    int execValue = 0;
    QDialogResult result = QDialogResult::Close;
    do
    {
        LoginDialog dlg; result = dlg.showDialog();
        if (result == QDialogResult::Ok) {
            MainWindow w(dlg.getSocket()); w.show();
            execValue = app.exec();
            result = w.windowStatus();
        }
    }while (result == QDialogResult::Ok || result == QDialogResult::Retry);

    return execValue;
}

