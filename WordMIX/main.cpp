#include "wordmix.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
  // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  // QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    // Splash screen
    QPixmap pixmap(":/WordMIX/WordMIXSplash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    WordMIXDialog wordMIX;
    wordMIX.setWindowIcon(QIcon(":/WordMIX/WordMIX.ico"));
    wordMIX.show();
    splash.finish(&wordMIX);

    return a.exec();
}
