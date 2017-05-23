#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include <QMessageBox>
#include <QRadioButton>

const int readBlock = 5;

MainWindow::MainWindow(QTcpSocket *tcp, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    result(QDialogResult::Ok), radio(nullptr), socket(tcp)
{
    ui->setupUi(this);

    init();

    nextQuestion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
   result = result == QDialogResult::Retry ?
               QDialogResult::Retry : QDialogResult::Close;
}

QDialogResult MainWindow::windowStatus()
{
    return result;
}

void MainWindow::exitWindow()
{
   result = QDialogResult::Retry;
   this->close();
}

void MainWindow::closeWindow()
{
   result = QDialogResult::Close;
   this->close();
}

void MainWindow::init()
{
    connect(ui->pb_select, SIGNAL(clicked(bool)), this, SLOT(enter()));
    connect(ui->rb_answer_1, SIGNAL(clicked(bool)), this, SLOT(select()));
    connect(ui->rb_answer_2, SIGNAL(clicked(bool)), this, SLOT(select()));
    connect(ui->rb_answer_3, SIGNAL(clicked(bool)), this, SLOT(select()));
    connect(ui->rb_answer_4, SIGNAL(clicked(bool)), this, SLOT(select()));
    connect(ui->act_exit, SIGNAL(triggered()), this, SLOT(exitWindow()));
    connect(ui->act_close, SIGNAL(triggered()), this, SLOT(closeWindow()));
    connect(ui->act_about, SIGNAL(triggered()),this, SLOT(showAboutDialog()));
}

void MainWindow::select()
{
    radio = qobject_cast<QRadioButton*>(sender());
}

void MainWindow::clearSelect()
{
    ui->rb_answer_1->setAutoExclusive(false);
    ui->rb_answer_1->setChecked(false);
    ui->rb_answer_1->setAutoExclusive(true);

    ui->rb_answer_2->setAutoExclusive(false);
    ui->rb_answer_2->setChecked(false);
    ui->rb_answer_2->setAutoExclusive(true);

    ui->rb_answer_3->setAutoExclusive(false);
    ui->rb_answer_3->setChecked(false);
    ui->rb_answer_3->setAutoExclusive(true);

    ui->rb_answer_4->setAutoExclusive(false);
    ui->rb_answer_4->setChecked(false);
    ui->rb_answer_4->setAutoExclusive(true);
}

bool MainWindow::nextQuestion()
{
    block = socket->readAll();
    QString text(block);

    QStringList data = text.split('@');
    if(data.count() == readBlock){
        ui->label->setText(data.takeFirst());
        ui->rb_answer_1->setText(data.takeFirst());
        ui->rb_answer_2->setText(data.takeFirst());
        ui->rb_answer_3->setText(data.takeFirst());
        ui->rb_answer_4->setText(data.takeFirst());
        return true;
    }

    evaluation = text.split('-').last();
    return false;
}


void MainWindow::enter()
{
    bool isChecked = ui->rb_answer_1->isChecked() ||  ui->rb_answer_2->isChecked() ||
            ui->rb_answer_3->isChecked() || ui->rb_answer_4->isChecked() ? true : false;

    if(!isChecked){
        QMessageBox::warning(this, "Тест", "Укажите вариант ответа");
        return;
    }

    if(writeSocketFrom(radio->text().toUtf8()) == 0)
    {
        clearSelect();
        if(!nextQuestion())
        {
            QMessageBox::information(this, "Тест", "Тестирование окончено");
            ui->label->setText(tr("Ваш результат: %1. Результат сохранен на сервере.").arg(evaluation));
            disconnect(ui->pb_select, SIGNAL(clicked(bool)), this, SLOT(enter()));
            ui->gb_answers->setDisabled(true);
            ui->pb_select->setDisabled(true);
        }
    }
}

int MainWindow::writeSocketFrom(const QByteArray &message)
{
    if (socket->write(message) <= 0)
    {
        QMessageBox::warning(this, tr("Клиент"), tr("Невозможно записать данные: %1")
                .arg(socket->errorString()));
        return -1;
    }

    if (!socket->waitForBytesWritten(3000)){
        QMessageBox::warning(this, tr("Клиент"), tr("Невозможно отправить данные: %1")
                         .arg(socket->errorString()));
        return -1;
    }

    if(!socket->waitForReadyRead(3000)) {

        QMessageBox::warning(this, tr("Клиент"), tr("Нет отклика от сервера: %1")
                     .arg(socket->errorString()));
        return -1;
    }

    return 0;
}

void MainWindow::showAboutDialog()
{
    AboutDialog dlg;
    dlg.exec();
}





