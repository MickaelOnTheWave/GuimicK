#ifdef _WIN32
   #include <Windows.h>
#endif   

#include <QApplication>

#include "editorversion.h"
#include "mainwindow.h"
#include "ostools.h"

void InitializeVersion()
{
#ifdef _WIN32
   EditorVersion::CreateAsClientVersion(OsTools::IsOnWindows());
#else
   EditorVersion::CreateAsDevelopmentVersion(OsTools::IsOnWindows());
#endif
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
   ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

   QApplication a(argc, argv);
   QCoreApplication::setOrganizationName("Task Manager");
   QCoreApplication::setApplicationName("Configuration Editor");

   InitializeVersion();
   MainWindow w;

   w.OpenDefaultFile();
   w.show();
   return a.exec();
}
