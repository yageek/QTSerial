#ifndef SERIALTOOL_H
#define SERIALTOOL_H

#include "ui_serialtool.h"
#include <qextserialport.h>
#include "readserial.h"
#include <QMutex>
class SerialTool : public QWidget, private Ui::SerialTool {
    Q_OBJECT
public:
    SerialTool(QWidget *parent = 0);
public slots:
    void openPort();
    void closePort();
    void printTerm(QByteArray);
    void writeCmd();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
  private:
    QextSerialPort *port;
    ReadSerial *Rth;
    QMutex mutex;
};

#endif // SERIALTOOL_H
