#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //socket_.readyRead();
    //socket_.connected();

    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);

    Activate();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::Activate(){
    if (socket_.state() == QAbstractSocket::UnconnectedState){
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        ui->checkSSL->setEnabled(true);
    }else{
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        ui->checkSSL->setEnabled(false);
    }
}



void Widget::doConnected()
{
    //socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    QString msg = "connected to " + socket_.peerAddress().toString() + "\r\n";
    ui->pteMessage->insertPlainText(msg);
}


void Widget::doDisconnected()
{
    QString msg = "Disconnected to " + socket_.peerAddress().toString() + "\r\n";
    ui->pteMessage->insertPlainText(msg);

}

void Widget::doReadyRead()
{
    QByteArray ba = socket_.readAll();
    ui->pteMessage->insertPlainText(ba);

}

void Widget::on_pbConnect_clicked()
{
    if (ui->checkSSL->isChecked()){
        socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    }else{
        socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    }

    Activate();
}


void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
}

void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}


void Widget::on_checkSSL_stateChanged(int arg1)
{
    if (arg1 == 2){
        ui->lePort->setText("443");
    }
    else{
        ui->lePort->setText("80");
    }
}

