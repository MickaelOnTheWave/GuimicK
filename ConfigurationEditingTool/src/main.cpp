#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

#ifdef _WIN32
   // Editing Server/Standalone configuration
   bool serverMode = true;
#else
   // Editing Remote Client configuration
   bool serverMode = false;
#endif

   MainWindow w(serverMode);
   w.show();

   return a.exec();
}
