#ifndef SERIALRAW_H
#define SERIALRAW_H

#include <QObject>
#include<QString>
#include<QByteArray>

class SerialRaw : public QObject
{
    Q_OBJECT
public:
    SerialRaw();
    QByteArray* getBuff();

public slots:
    void insertData(QByteArray data );
    QString getResult();
    void Clearbuffer();

private:
    QByteArray *buff;



};

#endif // SERIALRAW_H
