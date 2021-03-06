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
    this->serialraw = new SerialRaw();


    //Terminal
        this->terminal->setReadOnly(true);

    //Refresh the list of ports
        this->RefreshPort();


        this->loadLastParam();









}


void SerialTool::saveParam(){
    std::cout << tr("Enreg last parameters").toStdString() << std::endl;
    QSettings params("qtserial");
    params.beginGroup("last");
    QHashIterator<QString,int> currentparam(this->getParameters());

    while(currentparam.hasNext()){
        currentparam.next();

        params.setValue(currentparam.key(),currentparam.value());



    }

    //Save Last port USED
    params.setValue("port",this->portBox->currentText());
    //Save mode
    if(this->asciiRadio->isChecked()) params.setValue("mode","ascii");
    else if (this->terminalmodeRadio->isChecked()) params.setValue("mode","terminal");
    else params.setValue("mode","hayes");


    params.endGroup();
}

SerialTool::~SerialTool(){
this->saveParam();
    


    
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
    if(this->port != NULL) {
        std::cout << "Desallocation of previous configuration" << std::endl;
        delete this->port;

    }
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



void SerialTool::closePort(){

    this->openButton->setDisabled(false);
            this->closeButton->setDisabled(true);
            this->Configuration(true);

    port->close();

    Rth->Stop();
    Rth->wait();




}


void SerialTool::printTerm(QByteArray data){
    /*Insert into hexadecimal view*/
    this->hexplainText->insertPlainText(data.toHex());

    this->hexplainText->moveCursor(QTextCursor::End);
    this->hexplainText->ensureCursorVisible();
    /*Insert into ASCII view*/
    this->serialraw->insertData(data);
    this->terminal->setPlainText(this->serialraw->getResult());



   this->terminal->moveCursor(QTextCursor::End);
   this->terminal->ensureCursorVisible();

}

void SerialTool::writeCmd(){
    QString cmd = this->commandEdit->text();
    if(this->terminalmodeRadio->isChecked()) cmd.append('\n');
    else if(this->hayesRadio->isChecked()) cmd.append('\r');

    mutex.lock();
    this->port->write(cmd.toAscii(),cmd.length());
    mutex.unlock();
    this->commandEdit->clear();
}


void SerialTool::closeEvent(QCloseEvent*){
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
    this->serialraw->Clearbuffer();
    this->hexplainText->clear();
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
    case 115200: settings.BaudRate = BAUD115200;break;






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


QHash<QString,int> SerialTool::getParameters(){

    QHash<QString,int> param;
    /*Baudrate*/
    param["baudrate"] = this->baudrateComboBox->currentIndex();
    param["parity"] = this->parityComboBox->currentIndex();
    param["data_bits"] = this->bitsCombobox->currentIndex();
    param["stop_bits"] = this->stopbitsCombobox->currentIndex();
    param["flow_control"] = this->flowcontrolComboBox->currentIndex();

    return param;

}
void SerialTool::loadLastParam(){

    QSettings params("qtserial");

    this->baudrateComboBox->setCurrentIndex(params.value("last/baudrate").toInt());
    this->parityComboBox->setCurrentIndex(params.value("last/parity").toInt());
    this->bitsCombobox->setCurrentIndex(params.value("last/data_bits").toInt());
    this->stopbitsCombobox->setCurrentIndex(params.value("last/stop_bits").toInt());
   this->flowcontrolComboBox->setCurrentIndex(params.value("last/flow_control").toInt());

    this->portBox->insertItem(0,params.value("port").toString());

    QString mode = params.value("last/mode").toString();
    if(mode.compare("ascii") == 0) this->asciiRadio->setChecked(true);
    else if(mode.compare("terminal") == 0) this->terminalmodeRadio->setChecked(true);
    else this->hayesRadio->setChecked(true);

}
