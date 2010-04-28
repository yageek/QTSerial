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
    ~SerialTool();
public slots:
    void openPort();
    void closePort();
    void printTerm(QByteArray);
    void writeCmd();
    void RefreshPort();
    void ClearTerm();
    QString * getTerminalText();
    void setTerminalText(const QString);
    void UpdateTerminalMode();
    void Configuration(bool actived);
    PortSettings getConfiguration();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
  private:
    QextSerialPort *port;
    ReadSerial *Rth;
    QMutex mutex;
    QString *terminaltext;
};

#endif // SERIALTOOL_H
