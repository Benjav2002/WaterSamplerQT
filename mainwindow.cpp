#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"
//#include "btl1window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// Encontrar as portas seriais disponíveis
    qDebug() << "Portas seriais disponíveis:";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Nome:" << info.portName();
        qDebug() << "Descrição:" << info.description();
        qDebug() << "Fabricante:\n" << info.manufacturer();
    }

    COMPORT = new QSerialPort();
    //set COM port for USB Serial Connection
    COMPORT->setPortName("COM7");
    //set BaudRate for USB Serial Connection
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);



    if (COMPORT->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial Port Is Connected.";
        ui->connection->setProperty("state", "on");
        ui->lnonboard->show();
        ui->lnonboard_temp->show();
    } else {
        qDebug() << "Serial Port Is Not Connected." << COMPORT->errorString();
        ui->lnonboard->setVisible(false);
        ui->lnonboard_temp->setVisible(false);
    }

    COMPORT->setDataTerminalReady(true);
    connect(COMPORT, SIGNAL(readyRead()),this, SLOT(readSerialData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_btl_clicked()
{
    //btl1window Btl1window;
    //mainwindow can't be used while Btl1window is in use

    Btl1window = new btl1window(this);
    Btl1window->show();

}


void MainWindow::on_pushButton_clicked()
{
    if(COMPORT->isOpen()){
        writeSerialData(ui->lntest->text());
        //readToDebugPico(readSerialData());
        COMPORT->flush();
    }
}

void MainWindow::writeSerialData(const QString &data)
{
    if (COMPORT->isOpen() && COMPORT->isWritable()) {
        QByteArray byteArray = (data+'?').toUtf8();
        COMPORT->write(byteArray);
        qDebug() << "String escrita na porta serial:" << byteArray;
    } else {
        qDebug() << "Erro ao escrever na porta serial";
    }
}

QString MainWindow::readSerialData()
{
    if (COMPORT->isOpen()){
        while (COMPORT->bytesAvailable()) {
            Data_From_SerialPort += COMPORT->readAll();
            if (Data_From_SerialPort.at(Data_From_SerialPort.length() - 1) == '?'){
                IsDataReceived = true;
            }
        }
        if (IsDataReceived == true){
            qDebug() << "Data from Serial Port" << Data_From_SerialPort;
            IsDataReceived = false;
        }
    }
    readToDebugPico(Data_From_SerialPort);
    Data_From_SerialPort.clear();
/*
    QString receivedData;
    if (COMPORT->isOpen()) {
        QByteArray data;
        while (!data.contains('?')) {
            if (COMPORT->waitForReadyRead(2500)) { // Wait for data to be available (deve ser alterado conforme o tamanho da string a enviar)
                data.append(COMPORT->readAll());

            } else {
                // Handle timeout or error
                qDebug() << "Timeout or error while waiting for data.";
                return receivedData; // Return empty string
            }
        }
        receivedData = QString::fromUtf8(data);
        qDebug() << "String lida da porta serial:" << receivedData;
        readToDebugPico(data);
    }*/
    return Data_From_SerialPort;

}

void MainWindow::readToDebugPico(QString picomsg)
{
    QStringList lm = picomsg.split(u'?');
    for(const QString &msg : lm){
        if(msg == ""){
            //Do nothing
        }else{
            if(msg.startsWith("iTemp")){
                QStringList itl = msg.split(u'-');
                itl.pop_front(); //remove iTemp from list
                qDebug() << itl;
                ui->lnonboard_temp->setText(itl[0]); //print on UI the temp

            }else if(msg.startsWith("eTemp")){
                QStringList etl = msg.split(u'-');
                etl.pop_front(); //remove eTemp from list
                ui->lnexternal_temp->setText(etl[0]);//print on UI the temp

            }else if(msg.startsWith("LinAtuator")){
                QStringList lal = msg.split(u'-');
                //function to use the atuator
            }
        }
    }

}
