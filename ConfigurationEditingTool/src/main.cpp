#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   MainWindow w;
#ifdef _WIN32
   w.RestrictToStandaloneMode();
#endif
   w.show();
   return a.exec();
}
