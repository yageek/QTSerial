#include "serialraw.h"

SerialRaw::SerialRaw(QObject *parent):QThread(parent)
{
    /*Allocate the QByteArray*/
    this->buff = new QByteArray();
    this->continuer = true;

}

QByteArray* SerialRaw::getBuff(){


    return this->buff;
}

void SerialRaw::insertData(QByteArray data){
    
    /*Check all the characters*/
    for(int i = 0; i< data.size();i++){

        switch(data.at(i)){

            /*Backspace --> remove the last char in buff*/
        case 0x08:this->buff->chop(1);break;
        default:this->buff->append(data.at(i));break;

        }

   }
}

QString SerialRaw::getResult(){

return QString(*(this->getBuff()));


}

void SerialRaw::Clearbuffer(){

 this->buff->clear();

}
void SerialRaw::run(){

    while(continuer){



    }




}
void SerialRaw::Stop(){

this->continuer = false;
}
