#ifdef _WIN32
   #include <Windows.h>
#endif   

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef _WIN32
   ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

   QApplication a(argc, argv);
   QCoreApplication::setOrganizationName("Task Manager");
   QCoreApplication::setApplicationName("Configuration Editor");

   MainWindow w;
#ifdef _WIN32
   w.RestrictToStandaloneMode();
#endif

   w.OpenDefaultFile();
   w.show();
   return a.exec();
}
