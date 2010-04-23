#include "readserial.h"
#include<iostream>
#include<QString>
ReadSerial::ReadSerial(){

}
ReadSerial::ReadSerial(QextSerialPort *port,QObject *parent):QThread(parent)
{

    continuer = true;
    this->serial = port;

}

void ReadSerial::run(){
    std::cout << "Reading Thread "<< std::endl;
    while(continuer){




char buffer[BUFFSIZE];
int numBytes;
mutex.lock();
numBytes = serial->bytesAvailable();

    if(numBytes > 0){
            if(numBytes > BUFFSIZE) numBytes = BUFFSIZE;
                int i = serial->read(buffer, numBytes);
                std::cout << "Bytes read :  " << numBytes << std::endl;
                buffer[i] = 0;
       //  std::cout << buffer << std::endl;
         emit DataReady(QByteArray(buffer));
        std::cout << buffer << std::endl;
        }
    mutex.unlock();



    }
    std::cout << "End Reading Thread" << std::endl;

}

void ReadSerial::setContinue(bool continuer){
    this->continuer = continuer;

}

void ReadSerial::Stop(){
    this->setContinue(false);

}
