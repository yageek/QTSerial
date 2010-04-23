#ifndef READSERIAL_H
#define READSERIAL_H

#include <QThread>
#include <QMutex>
#include <qextserialport.h>
#define BUFFSIZE 2048
class ReadSerial : public QThread
{
Q_OBJECT
protected:
    void run();
public:
        ReadSerial();
    ReadSerial(QextSerialPort *port,QObject *parent=0);
     void setContinue(bool continuer);
        void Stop();
signals:
        void DataReady(QByteArray);
 private:
   bool continuer;
   QMutex mutex;
   QextSerialPort *serial;

};

#endif // READSERIAL_H
