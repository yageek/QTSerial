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


void SerialTool::closePort(){
           this->openButton->setDisabled(false);
            this->closeButton->setDisabled(true);


    port->close();

    Rth->Stop();
    Rth->wait();


}

void SerialTool::printTerm(QByteArray data){




    QString mesg;


    if(this->HexCheckBox->isChecked() == true){
        for(int i=0;i<data.size();i++){

               mesg.append(QString("0x%1  ").arg((char) data.at(i),(int) 0, (int) 16));


        }



    } else mesg.append(data);

    std::cout << mesg.toStdString() << std::endl;

    this->terminal->insertPlainText(mesg);
    this->terminal->ensureCursorVisible();


}

void SerialTool::writeCmd(){
    QString cmd = this->commandEdit->text();
    if(this->HexCheckBox->isChecked() == false) cmd.append("\n");

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
