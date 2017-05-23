#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog), result(QDialogResult::Close)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->pb_cencel, SIGNAL(clicked()), this, SLOT(cancelMethod()));
    connect(ui->pb_ok, SIGNAL(clicked()), this, SLOT(acceptMethod()));

    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        ui->cb_computer->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            ui->cb_computer->addItem(name + QChar('.') + domain);
    }
    if (name != QString("localhost"))
        ui->cb_computer->addItem(QString("localhost"));

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            ui->cb_computer->addItem(ipAddressesList.at(i).toString());
    }

    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            ui->cb_computer->addItem(ipAddressesList.at(i).toString());
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::name()
{
    return ui->le_name->text();
}

QDialogResult LoginDialog::showDialog()
{
   QDialog::exec();
   return result;
}

void LoginDialog::acceptMethod()
{
    if(ui->le_name->text().isEmpty()){
        QMessageBox::warning(this, "Клиент", tr("Укажите имя"));
        result = QDialogButtonBox::Retry;
    }

    else{
        socket.connectToHost(ui->cb_computer->currentText(), ui->sb_port->text().toInt());
        result = QDialogButtonBox::Ok;

        if ( !socket.waitForConnected(5000) ) {
            QMessageBox::warning(this, "Клиент", tr("Не удалось связаться с сервером: %1")
                                 .arg(socket.errorString()), QMessageBox::Ok);
            result = QDialogButtonBox::Retry;
        }

        writeSocketFrom(ui->le_name->text().toUtf8());

        if (!socket.waitForReadyRead(3000)) {
            QMessageBox::warning(this, "Клиент", tr("Нет отклика от сервера: %1")
                                 .arg(socket.errorString()), QMessageBox::Ok);
            result = QDialogButtonBox::Retry;
        }
    }

   accept();
}

void LoginDialog::cancelMethod()
{
   result = QDialogButtonBox::Close;
   close();
}

int LoginDialog::writeSocketFrom(const QByteArray &message)
{
    if (socket.write(message) <= 0)
    {
       QMessageBox::warning(this, "Клиент", tr("Невозможно записать данные: %1")
                .arg(socket.errorString()));
        return -1;
    }
    return 0;
}
