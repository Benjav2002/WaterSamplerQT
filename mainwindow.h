#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include "btl1window.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void on_btn_btl_clicked();
    void on_pushButton_clicked();
    void writeSerialData(const QString &data);
    QString readSerialData();
    void readToDebugPico(QString picomsg);

private:
    Ui::MainWindow *ui;
    QSerialPort *COMPORT;
    btl1window *Btl1window;
    QString Data_From_SerialPort;
    bool IsDataReceived = false;
};
#endif // MAINWINDOW_H
