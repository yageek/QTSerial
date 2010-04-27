#include "serialtool.h"
#include<QMessageBox>
#include "readserial.h"
#include<iostream>
#include <QByteArrayMatcher>
#include <qextserialenumerator.h>
SerialTool::SerialTool(QWidget *parent) :
    QWidget(parent){
    setupUi(this);

    this->closeButton->setDisabled(true);
    this->port = new QextSerialPort();
    port->setBaudRate(BAUD19200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    //Terminal
        this->terminal->setReadOnly(true);

    //Refresh the list of ports
        this->RefreshPort();
    //Initialise the terminal text
        this->terminaltext = new QString();



}

void SerialTool::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void SerialTool::openPort(){

    QString portName = this->portBox->currentText();
    port->setPortName(portName);

    if( port->open(QIODevice::ReadWrite) < 1){
    QMessageBox mesg;
    mesg.setText(QString("Erreur opening").append(portName));
    mesg.exec();



    }
    else{
            this->openButton->setDisabled(true);
            this->closeButton->setDisabled(false);
            this->Rth = new ReadSerial(port,this);
            connect(this->Rth,SIGNAL(DataReady(QByteArray)),this,SLOT(printTerm(QByteArray)));
            Rth->start();



        }
}

QString *SerialTool::getTerminalText(){

    return this->terminaltext;

}

void SerialTool::setTerminalText(QString text){

    this->terminaltext->clear();
    this->terminaltext->append(text);

}

void SerialTool::closePort(){
           this->openButton->setDisabled(false);
            this->closeButton->setDisabled(true);


    port->close();

    Rth->Stop();
    Rth->wait();


}



void SerialTool::UpdateTerminalMode(){

    if((this->asciiRadio->isChecked()) && !(this->hexRadio->isChecked())) this->terminal->setPlainText(*(this->getTerminalText()));


             else if(!(this->asciiRadio->isChecked()) && (this->hexRadio->isChecked())){

                     QString Hex;
                     for (int i = 0; i < this->getTerminalText()->size();i++){


                         Hex.append(QString("0x%1   ").arg(this->getTerminalText()->at(i).toAscii(),0,16));
                         this->terminal->setPlainText(Hex);

                     }


}

             this->terminal->ensureCursorVisible();
   }

void SerialTool::printTerm(QByteArray data){

/*Set data received in the class argumen */
   this->getTerminalText()->append(data);
    this->UpdateTerminalMode();


}

void SerialTool::writeCmd(){
    QString cmd = this->commandEdit->text();

    mutex.lock();
    this->port->write(cmd.toAscii(),cmd.length());
    mutex.unlock();
    this->commandEdit->clear();
}


void SerialTool::closeEvent(QCloseEvent *event){
if(port->isOpen())  this->closePort();
}

void SerialTool::RefreshPort(){
this->portBox->clear();

QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

 for (int i = 0; i < ports.size(); i++) {
    this->portBox->insertItem(i,ports.at(i).physName);
 }


}

void SerialTool::ClearTerm(){
    this->terminal->clear();
    this->getTerminalText()->clear();
}

