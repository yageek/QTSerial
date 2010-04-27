#include <QtGui/QApplication>
#include "serialtool.h"
#include<iostream>
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   SerialTool serial;
   serial.setVisible(true);

   return a.exec();
}
