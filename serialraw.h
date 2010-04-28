#ifndef SERIALRAW_H
#define SERIALRAW_H

#include <QObject>
#include<QString>
#include<QByteArray>
#include<QThread>
#include<QMutex>
class SerialRaw : public QThread
{
    Q_OBJECT
public:
    SerialRaw(QObject *parent=0);
    QByteArray* getBuff();

public slots:
    void insertData(QByteArray data );
    QString getResult();
    void Clearbuffer();

private:
    QByteArray *buff;
    QMutex mutex;
protected:
    void run();


};

#endif // SERIALRAW_H
