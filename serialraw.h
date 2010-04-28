#ifndef SERIALRAW_H
#define SERIALRAW_H

#include <QObject>
#include<QString>
#include<QByteArray>
#include<QThread>
#include<QMutex>
#include<QPlainTextEdit>
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
    void Stop();

private:
    QByteArray *buff;
    QPlainTextEdit *text;
    bool continuer;
protected:
    void run();


};

#endif // SERIALRAW_H
