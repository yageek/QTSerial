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
    /*Get actual configuration */

    QString portName = this->portBox->currentText();

    this->port = new QextSerialPort(portName,this->getConfiguration());

    if( port->open(QIODevice::ReadWrite) < 1){
    QMessageBox mesg;
    mesg.setText(QString("Erreur opening").append(portName));
    mesg.exec();



    }
    else{
            this->openButton->setDisabled(true);
            this->closeButton->setDisabled(false);
            /* Disable configuration button  */
            this->Configuration(false);

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
            this->Configuration(true);

    port->close();

    Rth->Stop();
    Rth->wait();



delete this->port;
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

#ifdef _TTY_POSIX_
    this->portBox->insertItem(i,ports.at(i).physName);
#elif _TTY_WIN_
    this->portBox->insertItem(i,ports.at(i).portName);
#endif
 }


}

void SerialTool::ClearTerm(){
    this->terminal->clear();
    this->getTerminalText()->clear();
}


void SerialTool::Configuration(bool actived){
    this->baudrateComboBox->setEnabled(actived);
    this->parityComboBox->setEnabled(actived);
    this->flowcontrolComboBox->setEnabled(actived);
    this->bitsCombobox->setEnabled(actived);
    this->stopbitsCombobox->setEnabled(actived);


}


PortSettings SerialTool::getConfiguration(){

     /*Baudrate*/
    PortSettings settings;

    switch(this->baudrateComboBox->currentText().toInt()){

    case 300: settings.BaudRate = BAUD300;break;
    case 600: settings.BaudRate = BAUD600;break;
    case 1200: settings.BaudRate = BAUD1200;break;
    case 2400: settings.BaudRate = BAUD2400;break;
    case 4800: settings.BaudRate = BAUD4800;break;
    case 9600: settings.BaudRate = BAUD9600;break;
    case 19200: settings.BaudRate = BAUD19200; break;
    case 38400: settings.BaudRate = BAUD38400; break;
    case 57600: settings.BaudRate = BAUD57600;break;






    }

    /*Bits*/

    switch(this->bitsCombobox->currentText().toInt()){

        case 5: settings.DataBits = DATA_5;break;
        case 6: settings.DataBits = DATA_6;break;
        case 7: settings.DataBits = DATA_7;break;
        case 8: settings.DataBits = DATA_8;break;


    }

    /*Control*/

    if(this->flowcontrolComboBox->currentText().compare("Hardware") == 0) settings.FlowControl = FLOW_HARDWARE;
    else  if(this->flowcontrolComboBox->currentText().compare("Software") == 0) settings.FlowControl = FLOW_XONXOFF;
    else settings.FlowControl = FLOW_OFF;

    /*StopBits*/

    if(this->stopbitsCombobox->currentText().toInt() == 2) settings.StopBits = STOP_2; else settings.StopBits = STOP_1;

    /*Parity*/
    if(this->parityComboBox->currentText().compare("Even") == 0)  settings.Parity = PAR_EVEN;
    else if(this->parityComboBox->currentText().compare("Odd") == 0) settings.Parity = PAR_ODD;
    else if(this->parityComboBox->currentText().compare("Space") == 0) settings.Parity = PAR_SPACE;
    else settings.Parity = PAR_NONE;

    return settings;
}
