#include <QtGui/QApplication>
#include "serialtool.h"
#include<iostream>
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   SerialTool teste;
   teste.setVisible(true);
/*    QRegExp rx;

     QString str = "[bleu] nimp [\\bleu]";
 rx.setPattern("\\[\\bbleu\\b\\]\\s*([a-z]+)\\s+\\[\\\\\\bbleu\\]");

 if (rx.exactMatch(str)) std::cout << " Vrai" << std::endl; else std::cout << "Faux" << std::endl;
 rx.indexIn(str);
 std::cout << rx.cap(1).toStdString() << std::endl;
  */
     return a.exec();
}
