#include "wordmix.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
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
