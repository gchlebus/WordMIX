#include "wordmix.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
 
    QApplication a(argc, argv);
    WordMIXDialog w;
    w.setWindowIcon(QIcon(":/WordMIX/WordMIX.ico"));
    w.show();
    return a.exec();
}
