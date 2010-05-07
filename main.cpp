#include <QtGui/QApplication>
#include "serialtool.h"
#include<iostream>
#include<QTranslator>
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   QString locale = QLocale::system().name().section('_', 0, 0);

     QTranslator translator;
     translator.load(QString("qtserial_") + locale);
     a.installTranslator(&translator);

   SerialTool serial;
   serial.setVisible(true);

   return a.exec();
}
