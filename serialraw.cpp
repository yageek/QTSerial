#include "serialraw.h"

SerialRaw::SerialRaw()
{
    /*Allocate the QByteArray*/
    this->buff = new QByteArray();

}

QByteArray* SerialRaw::getBuff(){


    return this->buff;
}

void SerialRaw::insertData(QByteArray data){
    
    /*Check all the characters*/
    for(int i = 0; i< data.size();i++){

        switch(data.at(i)){

            /*Backspace --> remove the last char in buff*/
        case 0x08:this->buff->remove(this->buff->size()-1,1);break;
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
