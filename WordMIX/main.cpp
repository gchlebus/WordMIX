#include "wordmix.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("WordMIX");
    QCoreApplication::setOrganizationName("gchlebus");

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
